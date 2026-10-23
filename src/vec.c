#include <stdint.h>
#include <string.h>

#include <libce/mem.h>
#include <libce/vec.h>

static CeResult ce_vec_grow(CeVec *vec, size_t needed)
{
    size_t capacity;
    size_t bytes;
    CeResult result;
    unsigned char *data;

    if (needed <= vec->cap)
        return CE_OK;

    capacity = vec->cap == 0 ? 8 : vec->cap;
    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    result = ce_size_mul(capacity, vec->elem_size, &bytes);
    if (result != CE_OK)
        return result;

    data = ce_mem_realloc(vec->data, bytes);
    if (data == NULL)
        return CE_OUT_OF_MEMORY;

    vec->data = data;
    vec->cap = capacity;
    return CE_OK;
}

CeResult ce_vec_init(CeVec *vec, size_t elem_size)
{
    if (vec == NULL || elem_size == 0)
        return CE_INVALID_ARGUMENT;

    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
    vec->elem_size = elem_size;
    return CE_OK;
}

void ce_vec_free(CeVec *vec)
{
    if (vec == NULL)
        return;

    ce_mem_free(vec->data);
    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
    vec->elem_size = 0;
}

void ce_vec_clear(CeVec *vec)
{
    if (vec != NULL)
        vec->len = 0;
}

CeResult ce_vec_reserve(CeVec *vec, size_t capacity)
{
    if (vec == NULL || vec->elem_size == 0)
        return CE_INVALID_ARGUMENT;

    return ce_vec_grow(vec, capacity);
}

CeResult ce_vec_resize(CeVec *vec, size_t len)
{
    size_t old_bytes;
    size_t new_bytes;
    CeResult result;

    if (vec == NULL || vec->elem_size == 0)
        return CE_INVALID_ARGUMENT;

    result = ce_vec_grow(vec, len);
    if (result != CE_OK)
        return result;
    if (len <= vec->len) {
        vec->len = len;
        return CE_OK;
    }

    result = ce_size_mul(vec->len, vec->elem_size, &old_bytes);
    if (result != CE_OK)
        return result;
    result = ce_size_mul(len, vec->elem_size, &new_bytes);
    if (result != CE_OK)
        return result;

    memset(vec->data + old_bytes, 0, new_bytes - old_bytes);
    vec->len = len;
    return CE_OK;
}

CeResult ce_vec_push(CeVec *vec, const void *value)
{
    size_t needed;
    size_t offset;
    CeResult result;

    if (vec == NULL || vec->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_size_add(vec->len, 1, &needed);
    if (result != CE_OK)
        return result;

    result = ce_vec_grow(vec, needed);
    if (result != CE_OK)
        return result;

    result = ce_size_mul(vec->len, vec->elem_size, &offset);
    if (result != CE_OK)
        return result;

    memcpy(vec->data + offset, value, vec->elem_size);
    vec->len = needed;
    return CE_OK;
}

CeResult ce_vec_pop(CeVec *vec, void *value)
{
    size_t offset;
    CeResult result;

    if (vec == NULL || vec->elem_size == 0 || value == NULL)
        return CE_INVALID_ARGUMENT;
    if (vec->len == 0)
        return CE_INVALID_DATA;

    result = ce_size_mul(vec->len - 1, vec->elem_size, &offset);
    if (result != CE_OK)
        return result;

    memcpy(value, vec->data + offset, vec->elem_size);
    vec->len -= 1;
    return CE_OK;
}

unsigned char *ce_vec_data(CeVec *vec)
{
    return vec == NULL ? NULL : vec->data;
}

const unsigned char *ce_vec_const_data(const CeVec *vec)
{
    return vec == NULL ? NULL : vec->data;
}

size_t ce_vec_len(const CeVec *vec)
{
    return vec == NULL ? 0 : vec->len;
}

size_t ce_vec_capacity(const CeVec *vec)
{
    return vec == NULL ? 0 : vec->cap;
}

size_t ce_vec_elem_size(const CeVec *vec)
{
    return vec == NULL ? 0 : vec->elem_size;
}
