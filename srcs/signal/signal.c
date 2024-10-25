/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/23 20:03:57 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ctrl_c_function(int signal)
{
	int	pid;

	pid = waitpid(-1, NULL, WNOHANG);
	g_interrupt_signal = TRUE;
	if (signal == SIGINT)
	{
		if (pid == -1)
		{
			rl_replace_line("", 1);
			ft_putstr_fd("\n", STDOUT_FILENO);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

static void	sigquit_function(int signal)
{
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit\n", STDOUT_FILENO);
}

void	signal_setup(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_set_exec(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, sigquit_function);
}

void signal_set_rl(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}
