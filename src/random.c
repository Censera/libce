#include <stddef.h>

#include <ce/random.h>

void ce_random_seed(CeRandom *random, uint64_t seed)
{
    if (random == NULL)
        return;
    random->state = seed == 0 ? UINT64_C(0x9e3779b97f4a7c15) : seed;
}

uint64_t ce_random_next(CeRandom *random)
{
    uint64_t value;

    if (random == NULL)
        return 0;

    value = random->state;
    value ^= value << 13;
    value ^= value >> 7;
    value ^= value << 17;
    random->state = value;
    return value;
}

uint32_t ce_random_u32(CeRandom *random)
{
    return (uint32_t)(ce_random_next(random) >> 32);
}
