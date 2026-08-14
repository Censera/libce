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

static inline CeView ce_view_make(const void *data, size_t len)
{
    CeView view;

    view.data = (const unsigned char *)data;
    view.len = len;
    return view;
}

#ifdef __cplusplus
}
#endif

#endif
