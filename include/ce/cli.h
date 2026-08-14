#ifndef CE_CLI_H
#define CE_CLI_H

#include <stddef.h>

#include <ce/core.h>
#include <ce/view.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CeCli {
    int argc;
    const char *const *argv;
} CeCli;

CeResult ce_cli_init(CeCli *cli, int argc, const char *const *argv);
int ce_cli_has(const CeCli *cli, CeView option);
CeResult ce_cli_value(const CeCli *cli, CeView option, CeView *out);

#ifdef __cplusplus
}
#endif

#endif
