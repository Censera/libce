#ifndef CE_STREAM_H
#define CE_STREAM_H

#include <stddef.h>
#include <stdio.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeStream {
    FILE *file;
} CeStream;

CeResult ce_stream_open(CeStream *stream, const char *path, const char *mode);
void ce_stream_close(CeStream *stream);
CeResult ce_stream_read(CeStream *stream, void *data, size_t len, size_t *read_len);
CeResult ce_stream_write(CeStream *stream, const void *data, size_t len, size_t *written_len);
CeResult ce_stream_flush(CeStream *stream);
int ce_stream_eof(const CeStream *stream);

#ifdef __cplusplus
}
#endif

#endif
