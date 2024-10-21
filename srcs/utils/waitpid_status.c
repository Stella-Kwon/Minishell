/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitpid_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 15:40:30 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (g_received_signal == 130)
			return (130);
		else if (g_received_signal == 131)
			return (131);
		else
			return (WTERMSIG(wstatus));
	}
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (FAIL);
}
