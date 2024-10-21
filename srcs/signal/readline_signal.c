/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:58:24 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:03:45 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	signal_readline(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_redisplay();
		ft_putstr_fd("^C\n", 2);
		g_received_signal = 130;
	}
}

void	readline_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = signal_readline;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("sigaction SIGINT");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		perror("sigaction SIGQUIT");
}
