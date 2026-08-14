#ifndef CE_VIEW_H
#define CE_VIEW_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeView {
    const unsigned char *data;
    size_t len;
} CeView;

#ifdef __cplusplus
}
#endif

#endif
