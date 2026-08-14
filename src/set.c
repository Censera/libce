#include <ce/set.h>

CeResult ce_set_init(CeSet *set, size_t key_size, CeHashFn hash, CeEqualFn equal)
{
    unsigned char value = 0;
    CeResult result;

    result = ce_map_init(set, key_size, sizeof(value), hash, equal);
    if (result != CE_OK)
        return result;

    return CE_OK;
}

void ce_set_free(CeSet *set)
{
    ce_map_free(set);
}

void ce_set_clear(CeSet *set)
{
    ce_map_clear(set);
}

CeResult ce_set_reserve(CeSet *set, size_t capacity)
{
    return ce_map_reserve(set, capacity);
}

CeResult ce_set_add(CeSet *set, const void *key)
{
    unsigned char value = 0;
    return ce_map_put(set, key, &value);
}

CeResult ce_set_has(const CeSet *set, const void *key, int *present)
{
    const void *value;
    CeResult result;

    if (present == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_map_get(set, key, &value);
    if (result == CE_NOT_FOUND) {
        *present = 0;
        return CE_OK;
    }
    if (result != CE_OK)
        return result;

    *present = 1;
    return CE_OK;
}

CeResult ce_set_remove(CeSet *set, const void *key)
{
    return ce_map_remove(set, key);
}

size_t ce_set_len(const CeSet *set)
{
    return ce_map_len(set);
}
