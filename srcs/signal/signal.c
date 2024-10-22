/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 23:10:46 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_interrupt_signal;

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

void	signal_setup(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
}
