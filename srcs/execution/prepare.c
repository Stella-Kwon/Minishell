/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 21:30:12 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/19 23:13:22 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	prepare_cmd(t_Command **command, int last_exitcode)
{
	int	argc;

	if (!command || !*command)
		return (0);
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

int	find_command_path(t_Command **command)
{
	char	*path;

	if (check_cmd_script(command) == FAIL || check_cmd_error(command) == FAIL)
		return (FAIL);
	if (find_and_check_path(command, &path) == FAIL)
		return (FAIL);
	free((*command)->cmd);
	(*command)->cmd = path;
	return (SUCCESS);
}

t_Command	*create_pipe_command(char ***env)
{
	t_Command	*res;

	res = (t_Command *)malloc(sizeof(t_Command));
	if (!res)
	{
		log_errors("Failed to malloc res in create_pipe_command", "");
		return (NULL);
	}
	res->env = env;
	res->cmd = NULL;
	res->args = NULL;
	res->exitcode = -1;
	res->wstatus = 0;
	res->root_node = NULL;
	return (res);
}
