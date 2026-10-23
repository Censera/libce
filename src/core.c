#include <libce/core.h>

const char *ce_result_name(CeResult result)
{
    switch (result) {
    case CE_OK: return "ok";
    case CE_INVALID_ARGUMENT: return "invalid argument";
    case CE_OUT_OF_MEMORY: return "out of memory";
    case CE_OVERFLOW: return "overflow";
    case CE_IO_ERROR: return "I/O error";
    case CE_NOT_FOUND: return "not found";
    case CE_EXISTS: return "already exists";
    case CE_INVALID_DATA: return "invalid data";
    default: return "unknown error";
    }
}
