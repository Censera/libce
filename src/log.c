#include <stdio.h>
#include <time.h>

#include <ce/log.h>

static const char *ce_log_level_name(CeLogLevel level)
{
    switch (level) {
    case CE_LOG_ERROR: return "error";
    case CE_LOG_WARN: return "warn";
    case CE_LOG_INFO: return "info";
    case CE_LOG_DEBUG: return "debug";
    default: return "unknown";
    }
}

void ce_logv(CeLogLevel level, const char *format, va_list args)
{
    if (format == NULL)
        return;
    fprintf(stderr, "[%s] ", ce_log_level_name(level));
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
}

void ce_log(CeLogLevel level, const char *message)
{
    if (message == NULL)
        return;
    fprintf(stderr, "[%s] %s\n", ce_log_level_name(level), message);
}

void ce_logf(CeLogLevel level, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    ce_logv(level, format, args);
    va_end(args);
}
