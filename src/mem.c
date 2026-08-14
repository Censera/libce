#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

#include <ce/mem.h>

CeResult ce_size_add(size_t left, size_t right, size_t *out)
{
    if (out == NULL)
        return CE_INVALID_ARGUMENT;
    if (right > SIZE_MAX - left)
        return CE_OVERFLOW;
    *out = left + right;
    return CE_OK;
}

CeResult ce_size_mul(size_t left, size_t right, size_t *out)
{
    if (out == NULL)
        return CE_INVALID_ARGUMENT;
    if (left != 0 && right > SIZE_MAX / left)
        return CE_OVERFLOW;
    *out = left * right;
    return CE_OK;
}

void *ce_mem_alloc(size_t size)
{
    return malloc(size);
}

void *ce_mem_calloc(size_t count, size_t size)
{
    return calloc(count, size);
}

void *ce_mem_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void ce_mem_free(void *ptr)
{
    free(ptr);
}
