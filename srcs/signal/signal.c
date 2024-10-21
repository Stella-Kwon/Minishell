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

void signal_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		g_received_signal = 10;
	}
}

void	execution_sigint(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^C\n", 2);

		g_received_signal = 130;
	}

}

void	execution_sigquit(int signal)
{
	if (signal == SIGQUIT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^\\Quit: 3\n", 2);
		g_received_signal = 131;
	}
}

void signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("SIGINT received.\n");
	}
	else if (signal == SIGQUIT)
	{
		printf("SIGQUIT received.\n");
	}
}

void signal_set(void (*sigint)(int), void (*sigquit)(int), int set_echoctl)
{
	struct sigaction sa;
	struct sigaction ga;

	set_termios(set_echoctl);
	ft_memset(&sa, 0, sizeof(sa));
	ft_memset(&ga, 0, sizeof(ga));
	sa.sa_handler = sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction SIGINT");
	}
	ga.sa_handler = sigquit;
	sigemptyset(&ga.sa_mask);
	ga.sa_flags = 0;
	if (sigaction(SIGQUIT, &ga, NULL) == -1)
	{
		perror("sigaction SIGQUIT");
	}
}
