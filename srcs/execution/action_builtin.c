/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 22:14:08 by skwon2             #+#    #+#             */
/*   Updated: 2024/10/22 20:26:28 by skwon2            ###   ########.fr       */
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
	if (redirect_infile_to_stdin(redir, cmd) == FAIL)
		return (FAIL);
	if (redirect_outfile_to_stdout(redir, cmd) == FAIL)
		return (FAIL);
	status = builtin(*cmd);
	restore_previous_fd(prev_stdout, prev_stdin);
	return (status);
}
