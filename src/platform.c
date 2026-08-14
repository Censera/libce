#include <ce/platform.h>

CePlatform ce_platform_current(void)
{
#ifdef _WIN32
    return CE_PLATFORM_WINDOWS;
#elif defined(__unix__) || defined(__APPLE__)
    return CE_PLATFORM_UNIX;
#else
    return CE_PLATFORM_UNKNOWN;
#endif
}

const char *ce_platform_name(CePlatform platform)
{
    switch (platform) {
    case CE_PLATFORM_WINDOWS:
        return "windows";
    case CE_PLATFORM_UNIX:
        return "unix";
    case CE_PLATFORM_UNKNOWN:
    default:
        return "unknown";
    }
}
