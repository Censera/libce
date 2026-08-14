#include <errno.h>

#include <ce/process.h>

#ifdef _WIN32
#include <process.h>
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

CeResult ce_process_run(const char *program, const char *const *argv, int *status)
{
    if (program == NULL || argv == NULL || status == NULL)
        return CE_INVALID_ARGUMENT;

#ifdef _WIN32
    int result = _spawnvp(_P_WAIT, program, argv);

    if (result == -1)
        return errno == ENOENT ? CE_NOT_FOUND : CE_IO_ERROR;

    *status = result;
    return CE_OK;
#else
    pid_t pid = fork();
    int code;

    if (pid < 0)
        return CE_IO_ERROR;

    if (pid == 0) {
        execvp(program, (char *const *)argv);
        _exit(127);
    }

    if (waitpid(pid, &code, 0) < 0)
        return CE_IO_ERROR;

    if (WIFEXITED(code)) {
        *status = WEXITSTATUS(code);
        return CE_OK;
    }

    if (WIFSIGNALED(code)) {
        *status = 128 + WTERMSIG(code);
        return CE_OK;
    }

    return CE_IO_ERROR;
#endif
}
