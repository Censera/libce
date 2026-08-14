#include <string.h>

#include <ce/path.h>

char ce_path_separator(void)
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

CeResult ce_path_join(CeStr *out, CeView left, CeView right)
{
    char separator;
    int need_separator;
    CeResult result;

    if (out == NULL || left.data == NULL || right.data == NULL)
        return CE_INVALID_ARGUMENT;

    separator = ce_path_separator();
    need_separator = left.len != 0 && left.data[left.len - 1] != separator;
    if (need_separator && right.len != 0 && right.data[0] == separator)
        need_separator = 0;

    result = ce_str_init_len(out, (const char *)left.data, left.len);
    if (result != CE_OK)
        return result;

    if (need_separator) {
        result = ce_str_append_char(out, separator);
        if (result != CE_OK) {
            ce_str_free(out);
            return result;
        }
    }

    result = ce_str_append_len(out, (const char *)right.data, right.len);
    if (result != CE_OK)
        ce_str_free(out);
    return result;
}

CeView ce_path_basename(CeView path)
{
    size_t start = path.len;

    if (path.data == NULL)
        return ce_view_make(NULL, 0);

    while (start > 0) {
        unsigned char c = path.data[start - 1];
        if (c == '/' || c == '\\')
            break;
        --start;
    }

    return ce_view_make(path.data + start, path.len - start);
}

CeView ce_path_extension(CeView path)
{
    CeView base = ce_path_basename(path);
    size_t dot = base.len;

    if (base.data == NULL)
        return base;

    while (dot > 0) {
        if (base.data[dot - 1] == '.')
            break;
        --dot;
    }

    if (dot == 0)
        return ce_view_make(NULL, 0);

    return ce_view_make(base.data + dot, base.len - dot);
}
