#ifndef CE_TERMINAL_H
#define CE_TERMINAL_H

#ifdef __cplusplus
extern "C" {
#endif

int ce_terminal_is_tty_stdin(void);
int ce_terminal_is_tty_stdout(void);
int ce_terminal_is_tty_stderr(void);

#ifdef __cplusplus
}
#endif

#endif
