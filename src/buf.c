#include <stdint.h>
#include <string.h>

#include <ce/buf.h>
#include <ce/mem.h>

static CeResult ce_buf_grow(CeBuf *buf, size_t needed)
{
    size_t capacity;

    if (buf->cap >= needed)
        return CE_OK;

    capacity = buf->cap == 0 ? 64 : buf->cap;
    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    return ce_buf_reserve(buf, capacity);
}

CeResult ce_buf_init(CeBuf *buf)
{
    if (buf == NULL)
        return CE_INVALID_ARGUMENT;
    buf->data = NULL;
    buf->len = 0;
    buf->cap = 0;
    return CE_OK;
}

void ce_buf_free(CeBuf *buf)
{
    if (buf == NULL)
        return;
    ce_mem_free(buf->data);
    buf->data = NULL;
    buf->len = 0;
    buf->cap = 0;
}

void ce_buf_clear(CeBuf *buf)
{
    if (buf == NULL)
        return;
    buf->len = 0;
}

CeResult ce_buf_reserve(CeBuf *buf, size_t capacity)
{
    unsigned char *data;

    if (buf == NULL)
        return CE_INVALID_ARGUMENT;
    if (capacity <= buf->cap)
        return CE_OK;

    data = ce_mem_realloc(buf->data, capacity);
    if (data == NULL)
        return CE_OUT_OF_MEMORY;

    buf->data = data;
    buf->cap = capacity;
    return CE_OK;
}

CeResult ce_buf_append(CeBuf *buf, const void *data, size_t len)
{
    size_t needed;
    CeResult result;

    if (buf == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    result = ce_size_add(buf->len, len, &needed);
    if (result != CE_OK)
        return result;
    result = ce_buf_grow(buf, needed);
    if (result != CE_OK)
        return result;

    if (len != 0)
        memcpy(buf->data + buf->len, data, len);
    buf->len = needed;
    return CE_OK;
}

CeResult ce_buf_append_byte(CeBuf *buf, unsigned char byte)
{
    return ce_buf_append(buf, &byte, 1);
}

unsigned char *ce_buf_data(CeBuf *buf)
{
    return buf == NULL ? NULL : buf->data;
}

const unsigned char *ce_buf_const_data(const CeBuf *buf)
{
    return buf == NULL ? NULL : buf->data;
}

size_t ce_buf_len(const CeBuf *buf)
{
    return buf == NULL ? 0 : buf->len;
}

size_t ce_buf_capacity(const CeBuf *buf)
{
    return buf == NULL ? 0 : buf->cap;
}
