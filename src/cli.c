#include <string.h>

#include <ce/cli.h>

static int ce_cli_match(const char *text, CeView option)
{
    if (text == NULL || option.data == NULL)
        return 0;

    return strlen(text) == option.len && memcmp(text, option.data, option.len) == 0;
}

CeResult ce_cli_init(CeCli *cli, int argc, const char *const *argv)
{
    if (cli == NULL || argc < 0 || (argc != 0 && argv == NULL))
        return CE_INVALID_ARGUMENT;

    cli->argc = argc;
    cli->argv = argv;
    return CE_OK;
}

int ce_cli_has(const CeCli *cli, CeView option)
{
    if (cli == NULL || option.data == NULL || option.len == 0)
        return 0;

    for (int i = 1; i < cli->argc; ++i) {
        if (ce_cli_match(cli->argv[i], option))
            return 1;
    }

    return 0;
}

CeResult ce_cli_value(const CeCli *cli, CeView option, CeView *out)
{
    if (cli == NULL || option.data == NULL || option.len == 0 || out == NULL)
        return CE_INVALID_ARGUMENT;

    for (int i = 1; i + 1 < cli->argc; ++i) {
        if (ce_cli_match(cli->argv[i], option)) {
            const char *value = cli->argv[i + 1];
            *out = ce_view_make(value, strlen(value));
            return CE_OK;
        }
    }

    return CE_NOT_FOUND;
}
