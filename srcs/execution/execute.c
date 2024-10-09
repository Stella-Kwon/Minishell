/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:20:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 16:33:24 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**find_env(char **envp)
{
	char	*path_var;

	path_var = NULL;
	if (*envp == NULL)
		return (ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':'));
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
		{
			path_var = *envp + 5;
			break ;
		}
		envp++;
	}
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

static char	*get_path(t_Command **command, char **env_path)
{
	char	*temp;
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	if (!env_path)
		cmd_error(command, ": No such file or directory\n", 127);
	while (env_path[i])
	{
		temp = ft_strjoin(env_path[i], "/");
		path = ft_strjoin(temp, (*command)->cmd);
		if (temp)
			free_one((void **)&temp);
		if (access(path, F_OK) == 0)
			break ;
		if (path)
			free_one((void **)&path);
		path = NULL;
		i++;
	}
	return (path);
}

static char	*find_and_check_path(t_Command **command)
{
	char	**env_path;
	char	*path;

	if (ft_strrchr((*command)->cmd, '/') != NULL)
		return ((*command)->cmd);
	env_path = find_env((*command)->env);
	path = get_path(command, env_path);
	{
		if (env_path != NULL)
			all_free(&env_path);
	}
	if (!path)
		return (NULL);
	check_path(path, command);
	return (path);
}

int prepare_cmd(t_Command **command, int last_exit_code)
{
	if (!command || !*command)
		return (SUCCESS);
	(*command)->cmd = expand_cmd((*command)->cmd, (*command)->env, \
									last_exit_code);
	(*command)->args = expand_args((*command)->args, (*command)->env, \
									last_exit_code);
	if (!(*command)->cmd)
		return (cmd_error(command, ": command not found\n", 127));
	return (SUCCESS);
}

int	execute_cmd(t_Command **command)
{
	char	*path;

	if (builtin_with_output(*command) == SUCCESS)
		return ((*command)->exitcode);
	if (check_cmd_script(command) == FAIL || check_cmd_error(command) == FAIL)
		return (FAIL);
	path = find_and_check_path(command);
	if (!path)
		return (cmd_error(command, ": command not found\n", 127));
	execve(path, (*command)->args, (*command)->env);
	if (path != (*command)->cmd)
		free_one((void **)&path);
	(*command)->exitcode = 127;
	return ((*command)->exitcode);
}
