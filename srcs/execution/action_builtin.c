/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:14:08 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/20 23:56:55 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
