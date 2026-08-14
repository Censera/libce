#ifndef CE_ENV_H
#define CE_ENV_H

#include <ce/core.h>
#include <ce/str.h>
#include <ce/view.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_env_get(CeView name, CeStr *out);
CeResult ce_env_set(CeView name, CeView value, int overwrite);
CeResult ce_env_unset(CeView name);

#ifdef __cplusplus
}
#endif

#endif
