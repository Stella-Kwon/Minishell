/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:19:22 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	action_child(t_Command *cmd, t_Redirection *redir, t_Pipeline *pipeline)
{
	if (redir->infile != -2)
	{
		if (dup2(redir->infile, STDIN_FILENO) == -1)
			return (log_errors("Failed to dup2 infile", ""));
		close(redir->infile);
	}
	if (redir->herestring_str)
	{
		if (here_string(&redir) != SUCCESS)
			return (FAIL);
		if (dup2(redir->infile, STDIN_FILENO) == -1)
			return (log_errors("Failed to dup2 herestring", ""));
		close(redir->infile);
	}
	if (redir->outfile != -2)
	{
		if (dup2(redir->outfile, STDOUT_FILENO) == -1)
			return (log_errors("Failed to dup2 outfile", ""));
		close(redir->outfile);
		close(pipeline->fd[0]);
		close(pipeline->fd[1]);
	}
	if (execute_cmd(cmd) == FAIL)
		log_errors("Execute_cmd has failed.", "");
	return (cmd->exitcode);
}
