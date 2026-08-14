#include <ce/hash.h>

uint64_t ce_hash_bytes(const void *data, size_t len)
{
    const unsigned char *bytes = data;
    uint64_t hash = UINT64_C(14695981039346656037);

    if (bytes == NULL && len != 0)
        return 0;

    for (size_t i = 0; i < len; ++i) {
        hash ^= bytes[i];
        hash *= UINT64_C(1099511628211);
    }

    return hash;
}
