#ifndef CE_FILE_H
#define CE_FILE_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeFile {
    unsigned char *data;
    size_t len;
} CeFile;

CeResult ce_file_read(const char *path, CeFile *out);
CeResult ce_file_write(const char *path, const void *data, size_t len);
CeResult ce_file_append(const char *path, const void *data, size_t len);
void ce_file_free(CeFile *file);

#ifdef __cplusplus
}
#endif

#endif
