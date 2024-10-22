/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 20:39:46 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	print_error_redirect(t_Command **cmd, char *filename, int redir_errno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	check_specific_error(filename, redir_errno);
	if (*cmd)
	{
		(*cmd)->exitcode = FAIL;
		exit(FAIL);
	}
	else
		return (2);
}


int	common_pre_child(t_Redirection	**redir, t_Command **cmd)
{
	if ((*redir)->infile != -2)
	{
		if ((*redir)->infile == -1)
		{
			print_error_redirect(cmd, (*redir)->in_filename, (*redir)->errno_in);
		}
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
		exit(FAIL);
	if ((*redir)->outfile != -2)
	{
		if ((*redir)->outfile == -1)
		{
			print_error_redirect(cmd, (*redir)->out_filename, (*redir)->errno_out);
		}
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
		{
			log_errors("Failed to redirect outfile in child process", \
						strerror(errno));
			(*cmd)->exitcode = FAIL;
			exit(FAIL);
		}
	}
	if (find_command_path(cmd) != SUCCESS)
		return ((*cmd)->exitcode);
	if (execve((*cmd)->cmd, (*cmd)->args, *((*cmd)->env)) == -1)
		exit(handle_error((*cmd)->cmd));
	exit(0);
}
