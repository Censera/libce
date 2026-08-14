#include <stdio.h>

#include <ce/format.h>
#include <ce/mem.h>

CeResult ce_str_vformat(CeStr *out, const char *format, va_list args)
{
    va_list copy;
    int needed;
    size_t length;
    size_t capacity;
    CeResult result;

    if (out == NULL || format == NULL)
        return CE_INVALID_ARGUMENT;

    va_copy(copy, args);
    needed = vsnprintf(NULL, 0, format, copy);
    va_end(copy);
    if (needed < 0)
        return CE_INVALID_DATA;

    length = (size_t)needed;
    result = ce_size_add(length, 1, &capacity);
    if (result != CE_OK)
        return result;

    result = ce_str_reserve(out, capacity);
    if (result != CE_OK)
        return result;

    va_copy(copy, args);
    if (vsnprintf(out->data, out->cap, format, copy) < 0) {
        va_end(copy);
        return CE_INVALID_DATA;
    }
    va_end(copy);
    out->len = length;
    return CE_OK;
}

CeResult ce_str_format(CeStr *out, const char *format, ...)
{
    va_list args;
    CeResult result;

    va_start(args, format);
    result = ce_str_vformat(out, format, args);
    va_end(args);
    return result;
}

CeResult ce_str_append_format(CeStr *out, const char *format, ...)
{
    CeStr temporary;
    va_list args;
    CeResult result;

    if (out == NULL || format == NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_str_init(&temporary, "");
    if (result != CE_OK)
        return result;

    va_start(args, format);
    result = ce_str_vformat(&temporary, format, args);
    va_end(args);
    if (result == CE_OK)
        result = ce_str_append_len(out, temporary.data, temporary.len);

    ce_str_free(&temporary);
    return result;
}
