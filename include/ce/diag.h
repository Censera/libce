#ifndef CE_DIAG_H
#define CE_DIAG_H

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *ce_result_message(CeResult result);

void ce_diag_print(CeResult result, const char *operation, const char *object);

#ifdef __cplusplus
}
#endif

#endif
