#ifndef CE_MEM_H
#define CE_MEM_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_size_add(size_t left, size_t right, size_t *out);
CeResult ce_size_mul(size_t left, size_t right, size_t *out);

void *ce_mem_alloc(size_t size);
void *ce_mem_calloc(size_t count, size_t size);
void *ce_mem_realloc(void *ptr, size_t size);
void ce_mem_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
