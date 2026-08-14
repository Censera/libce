#include <string.h>

#include <ce/dir.h>
#include <ce/mem.h>

#ifdef _WIN32
#include <windows.h>

typedef struct CeDirState {
    HANDLE find;
    WIN32_FIND_DATAA data;
    int first;
} CeDirState;
#else
#include <dirent.h>

typedef struct CeDirState {
    DIR *dir;
} CeDirState;
#endif

static int ce_dir_skip(const char *name)
{
    return strcmp(name, ".") == 0 || strcmp(name, "..") == 0;
}

CeResult ce_dir_open(CeDir *dir, const char *path)
{
    CeDirState *state;

    if (dir == NULL || path == NULL)
        return CE_INVALID_ARGUMENT;

    state = ce_mem_calloc(1, sizeof(*state));
    if (state == NULL)
        return CE_OUT_OF_MEMORY;

#ifdef _WIN32
    {
        size_t length = strlen(path);
        char *pattern;

        if (length > SIZE_MAX - 3) {
            ce_mem_free(state);
            return CE_OVERFLOW;
        }
        pattern = ce_mem_alloc(length + 3);
        if (pattern == NULL) {
            ce_mem_free(state);
            return CE_OUT_OF_MEMORY;
        }
        memcpy(pattern, path, length);
        if (length != 0 && pattern[length - 1] != '\\' && pattern[length - 1] != '/')
            pattern[length++] = '\\';
        pattern[length++] = '*';
        pattern[length] = '\0';

        state->find = FindFirstFileA(pattern, &state->data);
        ce_mem_free(pattern);
        if (state->find == INVALID_HANDLE_VALUE) {
            ce_mem_free(state);
            return CE_NOT_FOUND;
        }
        state->first = 1;
    }
#else
    state->dir = opendir(path);
    if (state->dir == NULL) {
        ce_mem_free(state);
        return CE_NOT_FOUND;
    }
#endif

    dir->handle = state;
    return CE_OK;
}

CeResult ce_dir_next(CeDir *dir, CeStr *name, int *done)
{
    CeDirState *state;

    if (dir == NULL || dir->handle == NULL || name == NULL || done == NULL)
        return CE_INVALID_ARGUMENT;

    state = dir->handle;

#ifdef _WIN32
    for (;;) {
        const char *entry;

        if (!state->first) {
            if (FindNextFileA(state->find, &state->data) == 0) {
                if (GetLastError() == ERROR_NO_MORE_FILES) {
                    *done = 1;
                    return CE_OK;
                }
                return CE_IO_ERROR;
            }
        }
        state->first = 0;
        entry = state->data.cFileName;
        if (ce_dir_skip(entry))
            continue;
        *done = 0;
        return ce_str_init(name, entry);
    }
#else
    for (;;) {
        struct dirent *entry = readdir(state->dir);
        if (entry == NULL) {
            if (errno != 0)
                return CE_IO_ERROR;
            *done = 1;
            return CE_OK;
        }
        if (ce_dir_skip(entry->d_name))
            continue;
        *done = 0;
        return ce_str_init(name, entry->d_name);
    }
#endif
}

void ce_dir_close(CeDir *dir)
{
    CeDirState *state;

    if (dir == NULL || dir->handle == NULL)
        return;

    state = dir->handle;
#ifdef _WIN32
    FindClose(state->find);
#else
    closedir(state->dir);
#endif
    ce_mem_free(state);
    dir->handle = NULL;
}
