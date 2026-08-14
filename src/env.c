#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include <ce/env.h>

static CeResult ce_env_name(CeView name, CeStr *out)
{
    if (name.data == NULL || name.len == 0 || out == NULL)
        return CE_INVALID_ARGUMENT;
    if (memchr(name.data, '\0', name.len) != NULL)
        return CE_INVALID_ARGUMENT;

    return ce_str_init_len(out, (const char *)name.data, name.len);
}

CeResult ce_env_get(CeView name, CeStr *out)
{
    CeStr key;
    const char *value;
    CeResult result;

    result = ce_env_name(name, &key);
    if (result != CE_OK)
        return result;

    value = getenv(key.data);
    ce_str_free(&key);
    if (value == NULL)
        return CE_NOT_FOUND;

    return ce_str_init(out, value);
}

CeResult ce_env_set(CeView name, CeView value, int overwrite)
{
    CeStr key;
    CeStr val;
    CeResult result;

    if (value.data == NULL || memchr(value.data, '\0', value.len) != NULL)
        return CE_INVALID_ARGUMENT;

    result = ce_env_name(name, &key);
    if (result != CE_OK)
        return result;
    result = ce_str_init_len(&val, (const char *)value.data, value.len);
    if (result != CE_OK) {
        ce_str_free(&key);
        return result;
    }

#ifdef _WIN32
    (void)overwrite;
    result = _putenv_s(key.data, val.data) == 0 ? CE_OK : CE_IO_ERROR;
#else
    result = setenv(key.data, val.data, overwrite != 0) == 0 ? CE_OK : CE_IO_ERROR;
#endif

    ce_str_free(&val);
    ce_str_free(&key);
    return result;
}

CeResult ce_env_unset(CeView name)
{
    CeStr key;
    CeResult result = ce_env_name(name, &key);

    if (result != CE_OK)
        return result;

#ifdef _WIN32
    result = _putenv_s(key.data, "") == 0 ? CE_OK : CE_IO_ERROR;
#else
    result = unsetenv(key.data) == 0 ? CE_OK : CE_IO_ERROR;
#endif

    ce_str_free(&key);
    return result;
}
