#ifndef CE_SORT_H
#define CE_SORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*CeCompareFn)(const void *left, const void *right);

void ce_sort(void *data, size_t count, size_t size, CeCompareFn compare);
void *ce_search(const void *key, const void *data, size_t count, size_t size, CeCompareFn compare);

#ifdef __cplusplus
}
#endif

#endif
