#ifndef CE_STR_H
#define CE_STR_H

#include <stddef.h>

#include <ce/core.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeStr {
    char *data;
    size_t len;
    size_t cap;
} CeStr;

CeResult ce_str_init(CeStr *str, const char *initial);
CeResult ce_str_init_len(CeStr *str, const char *data, size_t len);
void ce_str_free(CeStr *str);
void ce_str_clear(CeStr *str);

CeResult ce_str_reserve(CeStr *str, size_t capacity);
CeResult ce_str_append(CeStr *str, const char *text);
CeResult ce_str_append_len(CeStr *str, const char *data, size_t len);
CeResult ce_str_append_char(CeStr *str, char c);

const char *ce_str_data(const CeStr *str);
size_t ce_str_len(const CeStr *str);
size_t ce_str_capacity(const CeStr *str);

#ifdef __cplusplus
}
#endif

#endif
