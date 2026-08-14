#include <string.h>

#include <ce/mem.h>
#include <ce/str.h>

static CeResult ce_str_grow(CeStr *str, size_t needed)
{
    size_t capacity;

    if (str->cap >= needed)
        return CE_OK;

    capacity = str->cap == 0 ? 32 : str->cap;
    while (capacity < needed) {
        size_t next;
        CeResult result = ce_size_mul(capacity, 2, &next);
        if (result != CE_OK)
            return result;
        capacity = next;
    }

    return ce_str_reserve(str, capacity);
}

CeResult ce_str_init(CeStr *str, const char *initial)
{
    if (initial == NULL)
        return ce_str_init_len(str, "", 0);
    return ce_str_init_len(str, initial, strlen(initial));
}

CeResult ce_str_init_len(CeStr *str, const char *data, size_t len)
{
    if (str == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    str->data = NULL;
    str->len = 0;
    str->cap = 0;

    return ce_str_append_len(str, data, len);
}

void ce_str_free(CeStr *str)
{
    if (str == NULL)
        return;
    ce_mem_free(str->data);
    str->data = NULL;
    str->len = 0;
    str->cap = 0;
}

void ce_str_clear(CeStr *str)
{
    if (str == NULL)
        return;
    if (str->data != NULL)
        str->data[0] = '\0';
    str->len = 0;
}

CeResult ce_str_reserve(CeStr *str, size_t capacity)
{
    char *data;

    if (str == NULL)
        return CE_INVALID_ARGUMENT;
    if (capacity <= str->cap)
        return CE_OK;

    data = ce_mem_realloc(str->data, capacity);
    if (data == NULL)
        return CE_OUT_OF_MEMORY;

    str->data = data;
    str->cap = capacity;
    if (str->len == 0)
        str->data[0] = '\0';
    return CE_OK;
}

CeResult ce_str_append_len(CeStr *str, const char *data, size_t len)
{
    size_t needed;
    size_t allocation;
    CeResult result;

    if (str == NULL || (data == NULL && len != 0))
        return CE_INVALID_ARGUMENT;

    result = ce_size_add(str->len, len, &needed);
    if (result != CE_OK)
        return result;
    result = ce_size_add(needed, 1, &allocation);
    if (result != CE_OK)
        return result;
    result = ce_str_grow(str, allocation);
    if (result != CE_OK)
        return result;

    if (len != 0)
        memmove(str->data + str->len, data, len);
    str->len = needed;
    str->data[str->len] = '\0';
    return CE_OK;
}

CeResult ce_str_append(CeStr *str, const char *text)
{
    if (text == NULL)
        return CE_INVALID_ARGUMENT;
    return ce_str_append_len(str, text, strlen(text));
}

CeResult ce_str_append_char(CeStr *str, char c)
{
    return ce_str_append_len(str, &c, 1);
}

const char *ce_str_data(const CeStr *str)
{
    return str == NULL || str->data == NULL ? "" : str->data;
}

size_t ce_str_len(const CeStr *str)
{
    return str == NULL ? 0 : str->len;
}

size_t ce_str_capacity(const CeStr *str)
{
    return str == NULL ? 0 : str->cap;
}
