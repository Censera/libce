#ifndef CE_PROCESS_H
#define CE_PROCESS_H

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeProcess {
    int handle;
} CeProcess;

CeResult ce_process_run(const char *program, const char *const *argv, int *status);

#ifdef __cplusplus
}
#endif

#endif
