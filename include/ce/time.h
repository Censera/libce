#ifndef CE_TIME_H
#define CE_TIME_H

#include <stdint.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_time_now_ns(uint64_t *out);

#ifdef __cplusplus
}
#endif

#endif
