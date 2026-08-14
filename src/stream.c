#include <ce/stream.h>

CeResult ce_stream_open(CeStream *stream, const char *path, const char *mode)
{
    if (stream == NULL || path == NULL || mode == NULL)
        return CE_INVALID_ARGUMENT;

    stream->file = fopen(path, mode);
    if (stream->file == NULL)
        return CE_IO_ERROR;
    return CE_OK;
}

void ce_stream_close(CeStream *stream)
{
    if (stream == NULL || stream->file == NULL)
        return;
    fclose(stream->file);
    stream->file = NULL;
}

CeResult ce_stream_read(CeStream *stream, void *data, size_t len, size_t *read_len)
{
    size_t count;

    if (stream == NULL || stream->file == NULL || read_len == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    count = fread(data, 1, len, stream->file);
    *read_len = count;
    if (count < len && ferror(stream->file) != 0)
        return CE_IO_ERROR;
    return CE_OK;
}

CeResult ce_stream_write(CeStream *stream, const void *data, size_t len, size_t *written_len)
{
    size_t count;

    if (stream == NULL || stream->file == NULL || written_len == NULL ||
        (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    count = fwrite(data, 1, len, stream->file);
    *written_len = count;
    return count == len ? CE_OK : CE_IO_ERROR;
}

CeResult ce_stream_flush(CeStream *stream)
{
    if (stream == NULL || stream->file == NULL)
        return CE_INVALID_ARGUMENT;
    return fflush(stream->file) == 0 ? CE_OK : CE_IO_ERROR;
}

int ce_stream_eof(const CeStream *stream)
{
    return stream != NULL && stream->file != NULL && feof(stream->file) != 0;
}
