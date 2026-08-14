#ifndef CE_PATH_H
#define CE_PATH_H

#include <ce/core.h>
#include <ce/str.h>
#include <ce/view.h>

#ifdef __cplusplus
extern "C" {
#endif

char ce_path_separator(void);
CeResult ce_path_join(CeStr *out, CeView left, CeView right);
CeView ce_path_basename(CeView path);
CeView ce_path_extension(CeView path);

#ifdef __cplusplus
}
#endif

#endif
