#include <stdlib.h>

#include <ce/sort.h>

void ce_sort(void *data, size_t count, size_t size, CeCompareFn compare)
{
    if (count < 2 || size == 0 || data == NULL || compare == NULL)
        return;

    qsort(data, count, size, compare);
}

void *ce_search(const void *key, const void *data, size_t count, size_t size, CeCompareFn compare)
{
    if (key == NULL || data == NULL || count == 0 || size == 0 || compare == NULL)
        return NULL;

    return bsearch(key, data, count, size, compare);
}
