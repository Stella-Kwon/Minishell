/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:20:10 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/16 10:13:53 by hlee-sun         ###   ########.fr       */
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

static void	free_path(char *tmp, char *path)
{
	if (tmp)
	{
		free_one((void **)&tmp);
	}
	if (path)
	{
		free_one((void **)&path);
	}
}

static char	*get_path_in_loop(t_Command **command, char **env_path, char *path)
{
	int		i;
	char	*tmp;

	i = 0;
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
		if (access(path, F_OK) == 0)
			break ;
		free_path(tmp, path);
		i++;
	}
	return (path);
}

static char	*get_path(t_Command **command, char **env_path)
{
	char	*path;

	path = NULL;
	if (!env_path)
		cmd_error(command, ": No such file or directory\n", 127);
	return (get_path_in_loop(command, env_path, path));
}

int	find_and_check_path(t_Command **command, char **path)
{
	char	**env_path;

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
