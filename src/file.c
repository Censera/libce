#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include <ce/file.h>
#include <ce/mem.h>

static CeResult ce_file_read_stream(FILE *file, CeFile *out)
{
    unsigned char *data = NULL;
    size_t len = 0;
    size_t cap = 0;

    for (;;) {
        size_t room;
        size_t read_count;

        if (cap == len) {
            size_t next;
            unsigned char *next_data;

            if (cap == 0) {
                next = 4096;
            } else {
                if (cap > SIZE_MAX / 2) {
                    ce_mem_free(data);
                    return CE_OVERFLOW;
                }
                next = cap * 2;
            }

            next_data = ce_mem_realloc(data, next);
            if (next_data == NULL) {
                ce_mem_free(data);
                return CE_OUT_OF_MEMORY;
            }
            data = next_data;
            cap = next;
        }

        room = cap - len;
        read_count = fread(data + len, 1, room, file);
        len += read_count;

        if (read_count < room) {
            if (ferror(file)) {
                ce_mem_free(data);
                return CE_IO_ERROR;
            }
            break;
        }
    }

    if (len == 0) {
        ce_mem_free(data);
        data = NULL;
    }

    out->data = data;
    out->len = len;
    return CE_OK;
}

CeResult ce_file_read(const char *path, CeFile *out)
{
    FILE *file;
    CeResult result;

    if (path == NULL || out == NULL)
        return CE_INVALID_ARGUMENT;

    out->data = NULL;
    out->len = 0;

    file = fopen(path, "rb");
    if (file == NULL)
        return errno == ENOENT ? CE_NOT_FOUND : CE_IO_ERROR;

    result = ce_file_read_stream(file, out);
    if (fclose(file) != 0 && result == CE_OK) {
        ce_file_free(out);
        return CE_IO_ERROR;
    }
    return result;
}

CeResult ce_file_write(const char *path, const void *data, size_t len)
{
    FILE *file;

    if (path == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    file = fopen(path, "wb");
    if (file == NULL)
        return CE_IO_ERROR;

    if (len != 0 && fwrite(data, 1, len, file) != len) {
        fclose(file);
        return CE_IO_ERROR;
    }

    if (fclose(file) != 0)
        return CE_IO_ERROR;
    return CE_OK;
}

CeResult ce_file_append(const char *path, const void *data, size_t len)
{
    FILE *file;

    if (path == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    file = fopen(path, "ab");
    if (file == NULL)
        return CE_IO_ERROR;

    if (len != 0 && fwrite(data, 1, len, file) != len) {
        fclose(file);
        return CE_IO_ERROR;
    }

    if (fclose(file) != 0)
        return CE_IO_ERROR;
    return CE_OK;
}

void ce_file_free(CeFile *file)
{
    if (file == NULL)
        return;
    ce_mem_free(file->data);
    file->data = NULL;
    file->len = 0;
}
