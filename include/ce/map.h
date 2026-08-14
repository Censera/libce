#ifndef CE_MAP_H
#define CE_MAP_H

#include <stddef.h>
#include <stdint.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t (*CeHashFn)(const void *key);
typedef int (*CeEqualFn)(const void *left, const void *right);

typedef struct CeMap {
    unsigned char *entries;
    unsigned char *states;
    size_t len;
    size_t cap;
    size_t key_size;
    size_t value_size;
    size_t stride;
    CeHashFn hash;
    CeEqualFn equal;
} CeMap;

CeResult ce_map_init(CeMap *map, size_t key_size, size_t value_size,
                     CeHashFn hash, CeEqualFn equal);
void ce_map_free(CeMap *map);
void ce_map_clear(CeMap *map);

CeResult ce_map_reserve(CeMap *map, size_t capacity);
CeResult ce_map_put(CeMap *map, const void *key, const void *value);
CeResult ce_map_get(const CeMap *map, const void *key, const void **value);
CeResult ce_map_remove(CeMap *map, const void *key);

size_t ce_map_len(const CeMap *map);
size_t ce_map_capacity(const CeMap *map);

#ifdef __cplusplus
}
#endif

#endif
