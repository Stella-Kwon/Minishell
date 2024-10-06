/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:17:39 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	common_pre_child(t_Redirection	**redir)
{
	if ((*redir)->infile != -2)
	{
		if (dup2((*redir)->infile, STDIN_FILENO) == -1)
		{
			close((*redir)->infile);
			return (log_errors("Failed to dup2 in action_child", ""));
		}
		close((*redir)->infile);
	}
	if ((*redir)->herestring_str)
	{
		if (here_string(redir) != SUCCESS)
		{
			close((*redir)->infile);
			return (FAIL);
		}
		if (dup2((*redir)->infile, STDIN_FILENO) == -1)
		{
			close((*redir)->infile);
			return (log_errors("Failed to dup2 in action_child", ""));
		}
		close((*redir)->infile);
	}
	return (SUCCESS);
}

int	action_child(t_Command **cmd, t_Redirection **redir)
{
	if (common_pre_child(redir) == FAIL)
		return (FAIL);
	if ((*redir)->outfile != -2)
	{
		if (dup2((*redir)->outfile, STDOUT_FILENO) == -1)
		{
			close((*redir)->outfile);
			return (log_errors("Failed to dup2 in action_child", ""));
		}
		close((*redir)->outfile);
	}
	if (execute_cmd(cmd) == FAIL)
		log_errors("Execute_cmd has failed.", "");
	else
		exit(SUCCESS);
	exit((*cmd)->exitcode);
}
