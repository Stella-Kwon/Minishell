/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:01:19 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_received_signal = 0;

void	signal_no_input(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_received_signal = 1;
	}
}

void	signal_execution_process(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^C\n", 2);
		g_received_signal = 130;
	}
	else if (signal == SIGQUIT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^\\Quit: 3\n", 2);
		g_received_signal = 131;
	}
}

void	init_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_no_input;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction SIGINT");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction SIGQUIT");
}

void	init_execution_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_execution_process;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction SIGINT");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction SIGQUIT");
}
