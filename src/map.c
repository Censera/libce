#include <string.h>

#include <ce/map.h>
#include <ce/mem.h>

static size_t ce_map_slot(const CeMap *map, const void *key, size_t capacity)
{
    return (size_t)(map->hash(key) % capacity);
}

static unsigned char *ce_map_entry(const CeMap *map, size_t index)
{
    return map->entries + index * map->stride;
}

static CeResult ce_map_allocate(CeMap *map, size_t capacity)
{
    size_t stride;
    size_t bytes;
    unsigned char *entries;
    unsigned char *states;

    if (capacity == 0)
        return CE_INVALID_ARGUMENT;
    if (ce_size_add(map->key_size, map->value_size, &stride) != CE_OK)
        return CE_OVERFLOW;
    if (ce_size_mul(capacity, stride, &bytes) != CE_OK)
        return CE_OVERFLOW;

    entries = ce_mem_calloc(1, bytes);
    if (entries == NULL)
        return CE_OUT_OF_MEMORY;
    states = ce_mem_calloc(1, capacity);
    if (states == NULL) {
        ce_mem_free(entries);
        return CE_OUT_OF_MEMORY;
    }

    map->entries = entries;
    map->states = states;
    map->cap = capacity;
    map->stride = stride;
    return CE_OK;
}

static CeResult ce_map_rehash(CeMap *map, size_t capacity)
{
    unsigned char *old_entries = map->entries;
    unsigned char *old_states = map->states;
    size_t old_cap = map->cap;
    size_t old_len = map->len;
    CeResult result;

    map->entries = NULL;
    map->states = NULL;
    map->cap = 0;
    map->len = 0;

    result = ce_map_allocate(map, capacity);
    if (result != CE_OK) {
        map->entries = old_entries;
        map->states = old_states;
        map->cap = old_cap;
        map->len = old_len;
        return result;
    }

    for (size_t i = 0; i < old_cap; ++i) {
        size_t slot;
        unsigned char *old_entry;
        unsigned char *entry;

        if (old_states[i] != 1)
            continue;

        old_entry = old_entries + i * map->stride;
        slot = ce_map_slot(map, old_entry, map->cap);
        while (map->states[slot] == 1)
            slot = (slot + 1) % map->cap;

        entry = ce_map_entry(map, slot);
        memcpy(entry, old_entry, map->stride);
        map->states[slot] = 1;
        map->len += 1;
    }

    ce_mem_free(old_entries);
    ce_mem_free(old_states);
    return CE_OK;
}

static CeResult ce_map_find(const CeMap *map, const void *key, size_t *slot, int *found)
{
    size_t index;

    if (map->cap == 0)
        return CE_NOT_FOUND;

    index = ce_map_slot(map, key, map->cap);
    for (size_t probes = 0; probes < map->cap; ++probes) {
        unsigned char state = map->states[index];

        if (state == 0) {
            *slot = index;
            *found = 0;
            return CE_OK;
        }

        if (state == 1 && map->equal(key, ce_map_entry(map, index))) {
            *slot = index;
            *found = 1;
            return CE_OK;
        }

        index = (index + 1) % map->cap;
    }

    return CE_OVERFLOW;
}

CeResult ce_map_init(CeMap *map, size_t key_size, size_t value_size,
                     CeHashFn hash, CeEqualFn equal)
{
    if (map == NULL || key_size == 0 || value_size == 0 || hash == NULL || equal == NULL)
        return CE_INVALID_ARGUMENT;

    memset(map, 0, sizeof(*map));
    map->key_size = key_size;
    map->value_size = value_size;
    map->hash = hash;
    map->equal = equal;
    return CE_OK;
}

void ce_map_free(CeMap *map)
{
    if (map == NULL)
        return;

    ce_mem_free(map->entries);
    ce_mem_free(map->states);
    memset(map, 0, sizeof(*map));
}

void ce_map_clear(CeMap *map)
{
    if (map == NULL)
        return;

    if (map->states != NULL)
        memset(map->states, 0, map->cap);
    map->len = 0;
}

CeResult ce_map_reserve(CeMap *map, size_t capacity)
{
    if (map == NULL || map->key_size == 0 || map->value_size == 0)
        return CE_INVALID_ARGUMENT;
    if (capacity <= map->cap)
        return CE_OK;

    return ce_map_rehash(map, capacity);
}

CeResult ce_map_put(CeMap *map, const void *key, const void *value)
{
    size_t slot;
    int found;
    CeResult result;
    unsigned char *entry;

    if (map == NULL || key == NULL || value == NULL)
        return CE_INVALID_ARGUMENT;

    if (map->cap == 0) {
        result = ce_map_allocate(map, 8);
        if (result != CE_OK)
            return result;
    } else if ((map->len + 1) * 10 >= map->cap * 7) {
        if (map->cap > SIZE_MAX / 2)
            return CE_OVERFLOW;
        result = ce_map_rehash(map, map->cap * 2);
        if (result != CE_OK)
            return result;
    }

    result = ce_map_find(map, key, &slot, &found);
    if (result != CE_OK)
        return result;

    entry = ce_map_entry(map, slot);
    if (found) {
        memcpy(entry + map->key_size, value, map->value_size);
        return CE_OK;
    }

    memcpy(entry, key, map->key_size);
    memcpy(entry + map->key_size, value, map->value_size);
    map->states[slot] = 1;
    map->len += 1;
    return CE_OK;
}

CeResult ce_map_get(const CeMap *map, const void *key, const void **value)
{
    size_t slot;
    int found;
    CeResult result;

    if (map == NULL || key == NULL || value == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_map_find(map, key, &slot, &found);
    if (result != CE_OK)
        return result;
    if (!found)
        return CE_NOT_FOUND;

    *value = ce_map_entry(map, slot) + map->key_size;
    return CE_OK;
}

CeResult ce_map_remove(CeMap *map, const void *key)
{
    size_t slot;
    int found;
    CeResult result;

    if (map == NULL || key == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_map_find(map, key, &slot, &found);
    if (result != CE_OK)
        return result;
    if (!found)
        return CE_NOT_FOUND;

    map->states[slot] = 2;
    map->len -= 1;
    return CE_OK;
}

size_t ce_map_len(const CeMap *map)
{
    return map == NULL ? 0 : map->len;
}

size_t ce_map_capacity(const CeMap *map)
{
    return map == NULL ? 0 : map->cap;
}
