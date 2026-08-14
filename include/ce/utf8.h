#ifndef CE_UTF8_H
#define CE_UTF8_H

#include <stddef.h>
#include <stdint.h>

#include <ce/core.h>
#include <ce/view.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_utf8_validate(CeView text);
CeResult ce_utf8_next(CeView text, size_t *offset, uint32_t *codepoint);
size_t ce_utf8_width(uint32_t codepoint);

#ifdef __cplusplus
}
#endif

#endif
