#ifndef CE_BUF_H
#define CE_BUF_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeBuf {
    unsigned char *data;
    size_t len;
    size_t cap;
} CeBuf;

CeResult ce_buf_init(CeBuf *buf);
void ce_buf_free(CeBuf *buf);
void ce_buf_clear(CeBuf *buf);

CeResult ce_buf_reserve(CeBuf *buf, size_t capacity);
CeResult ce_buf_append(CeBuf *buf, const void *data, size_t len);
CeResult ce_buf_append_byte(CeBuf *buf, unsigned char byte);

unsigned char *ce_buf_data(CeBuf *buf);
const unsigned char *ce_buf_const_data(const CeBuf *buf);
size_t ce_buf_len(const CeBuf *buf);
size_t ce_buf_capacity(const CeBuf *buf);

#ifdef __cplusplus
}
#endif

#endif
