#ifndef CE_DIR_H
#define CE_DIR_H

#include <ce/core.h>
#include <ce/str.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeDir {
    void *handle;
} CeDir;

CeResult ce_dir_open(CeDir *dir, const char *path);
CeResult ce_dir_next(CeDir *dir, CeStr *name, int *done);
void ce_dir_close(CeDir *dir);

#ifdef __cplusplus
}
#endif

#endif
