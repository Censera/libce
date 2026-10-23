#ifndef CE_CORE_H
#define CE_CORE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CeResult {
    CE_OK = 0,
    CE_INVALID_ARGUMENT,
    CE_OUT_OF_MEMORY,
    CE_OVERFLOW,
    CE_IO_ERROR,
    CE_NOT_FOUND,
    CE_EXISTS,
    CE_INVALID_DATA
} CeResult;

const char *ce_result_name(CeResult result);

#ifdef __cplusplus
}
#endif

#endif
