#ifndef CE_HASH_H
#define CE_HASH_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t ce_hash_bytes(const void *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
