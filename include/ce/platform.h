#ifndef CE_PLATFORM_H
#define CE_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CePlatform {
    CE_PLATFORM_UNKNOWN = 0,
    CE_PLATFORM_WINDOWS,
    CE_PLATFORM_UNIX
} CePlatform;

CePlatform ce_platform_current(void);
const char *ce_platform_name(CePlatform platform);

#ifdef __cplusplus
}
#endif

#endif
