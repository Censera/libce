#include <stdint.h>

#include <ce/parse.h>

static int ce_digit(unsigned char c, unsigned base, unsigned *out)
{
    unsigned value;

    if (c >= '0' && c <= '9')
        value = (unsigned)(c - '0');
    else if (c >= 'a' && c <= 'z')
        value = (unsigned)(c - 'a') + 10;
    else if (c >= 'A' && c <= 'Z')
        value = (unsigned)(c - 'A') + 10;
    else
        return 0;

    if (value >= base)
        return 0;

    *out = value;
    return 1;
}

CeResult ce_parse_u64(CeView text, uint64_t *out, unsigned base)
{
    size_t i = 0;
    uint64_t value = 0;

    if (out == NULL || text.data == NULL || text.len == 0 || base < 2 || base > 36)
        return CE_INVALID_ARGUMENT;

    if (text.data[0] == '+') {
        i = 1;
        if (i == text.len)
            return CE_INVALID_DATA;
    } else if (text.data[0] == '-') {
        return CE_INVALID_DATA;
    }

    for (; i < text.len; ++i) {
        unsigned digit;

        if (!ce_digit(text.data[i], base, &digit))
            return CE_INVALID_DATA;
        if (value > (UINT64_MAX - digit) / base)
            return CE_OVERFLOW;
        value = value * base + digit;
    }

    *out = value;
    return CE_OK;
}

CeResult ce_parse_i64(CeView text, int64_t *out, unsigned base)
{
    size_t i = 0;
    int negative = 0;
    uint64_t value = 0;
    uint64_t limit;
    CeResult result;

    if (out == NULL || text.data == NULL || text.len == 0 || base < 2 || base > 36)
        return CE_INVALID_ARGUMENT;

    if (text.data[0] == '+' || text.data[0] == '-') {
        negative = text.data[0] == '-';
        i = 1;
        if (i == text.len)
            return CE_INVALID_DATA;
    }

    limit = negative ? (uint64_t)INT64_MAX + 1u : (uint64_t)INT64_MAX;
    for (; i < text.len; ++i) {
        unsigned digit;

        if (!ce_digit(text.data[i], base, &digit))
            return CE_INVALID_DATA;
        if (value > (limit - digit) / base)
            return CE_OVERFLOW;
        value = value * base + digit;
    }

    if (negative) {
        if (value == (uint64_t)INT64_MAX + 1u)
            *out = INT64_MIN;
        else
            *out = -(int64_t)value;
    } else {
        *out = (int64_t)value;
    }

    result = CE_OK;
    return result;
}

CeResult ce_parse_size(CeView text, size_t *out, unsigned base)
{
    uint64_t value;
    CeResult result = ce_parse_u64(text, &value, base);

    if (result != CE_OK)
        return result;
    if (value > SIZE_MAX)
        return CE_OVERFLOW;

    *out = (size_t)value;
    return CE_OK;
}
