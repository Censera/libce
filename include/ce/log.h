#ifndef CE_LOG_H
#define CE_LOG_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CeLogLevel {
    CE_LOG_ERROR,
    CE_LOG_WARN,
    CE_LOG_INFO,
    CE_LOG_DEBUG
} CeLogLevel;

void ce_log(CeLogLevel level, const char *message);
void ce_logf(CeLogLevel level, const char *format, ...);
void ce_logv(CeLogLevel level, const char *format, va_list args);

#ifdef __cplusplus
}
#endif

#endif
