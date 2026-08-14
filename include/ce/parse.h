#ifndef CE_PARSE_H
#define CE_PARSE_H

#include <stddef.h>
#include <stdint.h>

#include <ce/core.h>
#include <ce/view.h>

#ifdef __cplusplus
extern "C" {
#endif

CeResult ce_parse_u64(CeView text, uint64_t *out, unsigned base);
CeResult ce_parse_i64(CeView text, int64_t *out, unsigned base);
CeResult ce_parse_size(CeView text, size_t *out, unsigned base);

#ifdef __cplusplus
}
#endif

#endif
