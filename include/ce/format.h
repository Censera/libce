#ifndef CE_FORMAT_H
#define CE_FORMAT_H

#include <stdarg.h>

#include <ce/core.h>
#include <ce/str.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_str_vformat(CeStr *out, const char *format, va_list args);
CeResult ce_str_format(CeStr *out, const char *format, ...);
CeResult ce_str_append_format(CeStr *out, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
