#ifndef CE_SET_H
#define CE_SET_H

#include <ce/map.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef CeMap CeSet;

CeResult ce_set_init(CeSet *set, size_t key_size, CeHashFn hash, CeEqualFn equal);
void ce_set_free(CeSet *set);
void ce_set_clear(CeSet *set);
CeResult ce_set_reserve(CeSet *set, size_t capacity);
CeResult ce_set_add(CeSet *set, const void *key);
CeResult ce_set_has(const CeSet *set, const void *key, int *present);
CeResult ce_set_remove(CeSet *set, const void *key);
size_t ce_set_len(const CeSet *set);

#ifdef __cplusplus
}
#endif

#endif
