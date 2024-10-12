/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 17:33:56 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	common_pre_child(t_Redirection	**redir)
{
	if ((*redir)->infile != -2)
	{
		if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
			return (FAIL);
	}
	if ((*redir)->herestring_str)
	{
		if (here_string(redir) != SUCCESS)
			return (FAIL);
	if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
		return (FAIL);
	}
	return (SUCCESS);
}

int	action_child(t_Command **cmd, t_Redirection **redir)
{
	if (common_pre_child(redir) == FAIL)
		return (FAIL);
	if ((*redir)->outfile != -2)
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
			return (FAIL);
	execute_cmd(cmd);
	exit((*cmd)->exitcode);
}
