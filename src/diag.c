#include <stdio.h>

#include <ce/diag.h>

const char *ce_result_message(CeResult result)
{
    switch (result) {
    case CE_OK:
        return "operation succeeded";
    case CE_INVALID_ARGUMENT:
        return "invalid argument";
    case CE_OUT_OF_MEMORY:
        return "out of memory";
    case CE_OVERFLOW:
        return "size overflow";
    case CE_IO_ERROR:
        return "I/O error";
    case CE_NOT_FOUND:
        return "not found";
    case CE_EXISTS:
        return "already exists";
    case CE_INVALID_DATA:
        return "invalid data";
    default:
        return "unknown error";
    }
}

void ce_diag_print(CeResult result, const char *operation, const char *object)
{
    if (operation == NULL || object == NULL)
        return;

    fprintf(stderr, "%s %s: %s\n", operation, object, ce_result_message(result));
}
