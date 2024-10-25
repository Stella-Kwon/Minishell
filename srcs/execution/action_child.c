/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/23 21:06:41 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_error_redir(t_Command **cmd, char *filename, int redir_errno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	check_specific_error(filename, redir_errno);
	if (cmd && *cmd)
	{
		(*cmd)->exitcode = FAIL;
		
	}
	return (FAIL);
}

int	common_pre_child(t_Redirection	**redir, t_Command **cmd)
{
	if ((*redir)->infile != -2)
	{
		if ((*redir)->infile == -1)
			return (print_error_redir(cmd, (*redir)->in_filename, \
										(*redir)->errno_in));
		if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
		{
			log_errors("Failed to redirect infile", strerror(errno));
			return (FAIL);
		}
	}
	if ((*redir)->herestring_str)
	{
		if (here_string(redir) != SUCCESS)
			return (FAIL);
		if ((*redir)->infile != -2 && \
			dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
		{
			log_errors("Failed to redirect infile after herestring", \
						strerror(errno));
			return (FAIL);
		}
	}
	return (SUCCESS);
}

int	action_child(t_Command **cmd, t_Redirection **redir)
{
	if (common_pre_child(redir, cmd) == FAIL)
		free_exit(cmd, FAIL);
	if ((*redir)->outfile != -2)
	{
		if ((*redir)->outfile == -1)
		{
			if (print_error_redir(cmd, (*redir)->out_filename, \
									(*redir)->errno_out) == FAIL)
				free_exit(cmd, FAIL);
		}
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
		{
			log_errors("Failed to redirect outfile in child process", \
						strerror(errno));
			free_exit(cmd, FAIL);
		}
	}
	if (execve((*cmd)->cmd, (*cmd)->args, *((*cmd)->env)) == -1)
		free_exit(cmd, handle_error((*cmd)->cmd));
	exit(0);
}
