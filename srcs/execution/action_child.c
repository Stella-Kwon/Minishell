/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 00:03:54 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_error_redirect(t_Command **cmd, char *filename)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
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
			print_error_redirect(cmd, (*redir)->in_filename);
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
			print_error_redirect(cmd, (*redir)->out_filename);
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
		{
			log_errors("Failed to redirect outfile in child process", \
						strerror(errno));
			(*cmd)->exitcode = FAIL;
			exit(FAIL);
		}
	}
	if (execve((*cmd)->cmd, (*cmd)->args, *((*cmd)->env)) == -1)
		exit(handle_error((*cmd)->cmd));
	exit(0);
}

static void	restore_previous_fd(int prev_stdout, int prev_stdin)
{
	if (prev_stdout != -1)
	{
		if (dup2(prev_stdout, STDOUT_FILENO) == -1)
		{
			log_errors("Failed to restore STDOUT after builtin", \
						strerror(errno));
		}
		close(prev_stdout);
	}
	if (prev_stdin != -1)
	{
		if (dup2(prev_stdin, STDIN_FILENO) == -1)
			log_errors("Failed to restore STDIN after builtin", \
						strerror(errno));
		close(prev_stdin);
	}
}

int	action_builtin(t_Command **cmd, t_Redirection **redir)
{
	int	status;
	int	prev_stdout;
	int	prev_stdin;

	prev_stdout = dup(STDOUT_FILENO);
	prev_stdin = dup(STDIN_FILENO);
	if (common_pre_child(redir, cmd) == FAIL)
		return (FAIL);
	if ((*redir)->outfile != -2)
	{
		if ((*redir)->outfile == -1)
			print_error_redirect(cmd, (*redir)->out_filename);
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
		{
			log_errors("Failed to redirect outfile in child process", \
						strerror(errno));
			(*cmd)->exitcode = FAIL;
			return (FAIL);
		}
	}
	status = builtin(*cmd);
	restore_previous_fd(prev_stdout, prev_stdin);
	return (status);
}
