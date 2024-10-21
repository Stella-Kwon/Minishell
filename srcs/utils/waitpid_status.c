/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitpid_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/20 17:23:59 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_signal(int wstatus)
{
	if (g_received_signal == SIGINT)
	{
		g_received_signal = 0;
		return (130);	
	}
	else if (g_received_signal == SIGQUIT)
	{
		g_received_signal = 0;
		return (131);
	}
	return (128 + WTERMSIG(wstatus));
}
int	waitpid_status(int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 0)
			return (SUCCESS);
		else
			return (WEXITSTATUS(wstatus));
	}
	else if (WIFSIGNALED(wstatus))
	{
		return (check_signal(wstatus));
	}
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (FAIL);
}
