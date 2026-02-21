#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/signal.h>
#include <signal.h>
#include <setjmp.h>

extern volatile sig_atomic_t g_interrupt_signal;
extern volatile sig_atomic_t g_no_child;
extern volatile sig_atomic_t g_child_count;
extern sigjmp_buf g_readline_jmp_buf;
extern volatile sig_atomic_t g_readline_jmp_active;

void signal_set_exec(void);
void signal_setup(void);
void signal_set_rl(void);

#endif