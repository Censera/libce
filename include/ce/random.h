#ifndef CE_RANDOM_H
#define CE_RANDOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeRandom {
    uint64_t state;
} CeRandom;

void ce_random_seed(CeRandom *random, uint64_t seed);
uint64_t ce_random_next(CeRandom *random);
uint32_t ce_random_u32(CeRandom *random);

#ifdef __cplusplus
}
#endif

#endif
