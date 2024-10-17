/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:05 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/17 13:43:26 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	action_parents(t_Redirection **redir, t_Pipeline **pipeline, \
					t_Command **cmd)
{
	int	wstatus;

	wstatus = -2;
	if (waitpid((*pipeline)->pid, &wstatus, 0) == -1)
	{
		(*cmd)->exitcode = waitpid_status(wstatus);
		return ((*cmd)->exitcode);
	}
	if ((*redir)->outfile >= 0)
		close((*redir)->outfile);
	if ((*redir)->infile >= 0)
		close((*redir)->infile);
	(*cmd)->exitcode = waitpid_status(wstatus);
	return ((*cmd)->exitcode);
}

int	prepare_cmd(t_Command **command, int last_exitcode)
{
	int	argc;

	if (!command || !*command)
		return (SUCCESS);
	if (ft_strncmp((*command)->cmd, "export", 7) == 0)
	{
		argc = get_str_len((*command)->args);
		merge_quoted_args((*command)->args, &argc);
	}
	expand_cmd_args(*command, last_exitcode);
	if (!(*command)->cmd)
		return (cmd_error(command, ": command not found\n", 127));
	if (handle_empty_cmd(command) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

int	execute_cmd(t_Command **command)
{
	char	*path;

	if (builtin_with_output(*command) == SUCCESS)
		return (SUCCESS);
	if (check_cmd_script(command) == FAIL || check_cmd_error(command) == FAIL)
		return (FAIL);
	if (find_and_check_path(command, &path) == FAIL)
		return (FAIL);
	if (execve(path, (*command)->args, *((*command)->env)) == -1)
	{
		handle_error(command, path);
	}
	if (path != (*command)->cmd)
		free_one((void **)&path);
	return ((*command)->exitcode);
}
