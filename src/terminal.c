#include <stdio.h>

#include <ce/terminal.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

int ce_terminal_is_tty_stdin(void)
{
#ifdef _WIN32
    return _isatty(_fileno(stdin));
#else
    return isatty(STDIN_FILENO);
#endif
}

int ce_terminal_is_tty_stdout(void)
{
#ifdef _WIN32
    return _isatty(_fileno(stdout));
#else
    return isatty(STDOUT_FILENO);
#endif
}

int ce_terminal_is_tty_stderr(void)
{
#ifdef _WIN32
    return _isatty(_fileno(stderr));
#else
    return isatty(STDERR_FILENO);
#endif
}
