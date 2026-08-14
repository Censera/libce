#include <stdint.h>
#include <time.h>

#include <ce/time.h>

CeResult ce_time_now_ns(uint64_t *out)
{
    struct timespec now;
    uint64_t seconds;

    if (out == NULL)
        return CE_INVALID_ARGUMENT;
    if (timespec_get(&now, TIME_UTC) != TIME_UTC)
        return CE_IO_ERROR;
    if (now.tv_sec < 0 || (uintmax_t)now.tv_sec > UINT64_MAX / UINT64_C(1000000000))
        return CE_OVERFLOW;
    if (now.tv_nsec < 0 || now.tv_nsec >= 1000000000L)
        return CE_INVALID_DATA;

    seconds = (uint64_t)now.tv_sec;
    if (seconds > (UINT64_MAX - (uint64_t)now.tv_nsec) / UINT64_C(1000000000))
        return CE_OVERFLOW;

    *out = seconds * UINT64_C(1000000000) + (uint64_t)now.tv_nsec;
    return CE_OK;
}
