/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:44:52 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <setjmp.h>

sigjmp_buf g_readline_jmp_buf;
volatile sig_atomic_t g_readline_jmp_active = FALSE;

static void ctrl_c_function(int signal)
{
	g_interrupt_signal = TRUE;
	if (signal == SIGINT)
	{
		if (g_no_child == TRUE)
		{
			rl_replace_line("", 1);
			ft_putstr_fd("\n", STDOUT_FILENO);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	g_interrupt_signal = FALSE;
}

static void ctrl_c_function_rl(int signal)
{
	(void)signal;
	g_interrupt_signal = TRUE;
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (g_readline_jmp_active)
	{
		g_readline_jmp_active = FALSE;
		siglongjmp(g_readline_jmp_buf, 1);
	}
}

static void sigquit_function(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
}

void signal_setup(void)
{
	g_interrupt_signal = FALSE;
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
}

void signal_set_exec(void)
{
	if (g_interrupt_signal == TRUE)
		g_interrupt_signal = FALSE;
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, sigquit_function);
}

void signal_set_rl(void)
{
	signal(SIGINT, ctrl_c_function_rl);
	signal(SIGQUIT, SIG_IGN);
}
