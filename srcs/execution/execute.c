/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:20:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/15 22:49:47 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char **find_env(char **envp)
{
	char *path_var;

	path_var = NULL;
	if (*envp == NULL)
		return (ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':'));
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
		{
			path_var = *envp + 5;
			break;
		}
		envp++;
	}
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

static char *get_path(t_Command **command, char **env_path)
{
	char *tmp;
	char *path;
	int i;

	i = 0;
	path = NULL;
	if (!env_path)
		cmd_error(command, ": No such file or directory\n", 127);
	while (env_path[i])
	{
		tmp = ft_strjoin(env_path[i], "/");
		if (!tmp)
		{
			log_errors("Failed strjoin in get_path, temp", "");
			return (NULL);
		}
		path = ft_strjoin(tmp, (*command)->cmd);
		if (!path)
		{
			log_errors("Failed strjoin in get_path, path", "");
			return (NULL);
		}
		if (tmp)
			free_one((void **)&tmp);
		if (access(path, F_OK) == 0)
			break;
		if (path)
			free_one((void **)&path);
		path = NULL;
		i++;
	}
	return (path);
}

static int find_and_check_path(t_Command **command, char **path)
{
	char **env_path;

	if (ft_strrchr((*command)->cmd, '/') != NULL)
	{
		*path = (*command)->cmd;
		return (check_path(*path, command));
	}
	env_path = find_env(*((*command)->env));
	*path = get_path(command, env_path);
	{
		if (env_path != NULL)
			all_free(&env_path);
	}
	if (!(*path))
		return (cmd_error(command, ": command not found\n", 127));
	return (check_path(*path, command));
}

int prepare_cmd(t_Command **command, int last_exitcode)
{
	int argc;

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

int execute_cmd(t_Command **command)
{
	char *path;

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
