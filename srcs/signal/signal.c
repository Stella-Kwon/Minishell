/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 21:55:43 by hlee-sun         ###   ########.fr       */
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

void	signal_setup(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_set_exec(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_DFL);
}
