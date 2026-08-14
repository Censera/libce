#include <ce/utf8.h>

static int ce_utf8_is_continuation(unsigned char byte)
{
    return (byte & 0xc0u) == 0x80u;
}

size_t ce_utf8_width(uint32_t codepoint)
{
    if (codepoint <= 0x7fu)
        return 1;
    if (codepoint <= 0x7ffu)
        return 2;
    if (codepoint <= 0xffffu)
        return 3;
    if (codepoint <= 0x10ffffu)
        return 4;
    return 0;
}

CeResult ce_utf8_next(CeView text, size_t *offset, uint32_t *codepoint)
{
    size_t index;
    size_t width;
    uint32_t value;
    unsigned char first;

    if (offset == NULL || codepoint == NULL || text.data == NULL)
        return CE_INVALID_ARGUMENT;

    index = *offset;
    if (index >= text.len)
        return CE_NOT_FOUND;

    first = text.data[index];
    if (first <= 0x7fu) {
        *codepoint = first;
        *offset = index + 1;
        return CE_OK;
    }

    if (first >= 0xc2u && first <= 0xdfu)
        width = 2;
    else if (first >= 0xe0u && first <= 0xefu)
        width = 3;
    else if (first >= 0xf0u && first <= 0xf4u)
        width = 4;
    else
        return CE_INVALID_DATA;

    if (index > text.len - width)
        return CE_INVALID_DATA;

    if (!ce_utf8_is_continuation(text.data[index + 1]))
        return CE_INVALID_DATA;
    if (width >= 3 && !ce_utf8_is_continuation(text.data[index + 2]))
        return CE_INVALID_DATA;
    if (width == 4 && !ce_utf8_is_continuation(text.data[index + 3]))
        return CE_INVALID_DATA;

    value = first & (width == 2 ? 0x1fu : width == 3 ? 0x0fu : 0x07u);
    for (size_t i = 1; i < width; ++i)
        value = (value << 6) | (text.data[index + i] & 0x3fu);

    if (value > 0x10ffffu || (value >= 0xd800u && value <= 0xdfffu))
        return CE_INVALID_DATA;
    if ((width == 2 && value < 0x80u) || (width == 3 && value < 0x800u) ||
        (width == 4 && value < 0x10000u))
        return CE_INVALID_DATA;

    *codepoint = value;
    *offset = index + width;
    return CE_OK;
}

CeResult ce_utf8_validate(CeView text)
{
    size_t offset = 0;
    uint32_t codepoint;

    if (text.data == NULL && text.len != 0)
        return CE_INVALID_ARGUMENT;

    while (offset < text.len) {
        CeResult result = ce_utf8_next(text, &offset, &codepoint);

        if (result != CE_OK)
            return result;
    }

    return CE_OK;
}
