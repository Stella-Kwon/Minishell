/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:20:10 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/22 20:11:27 by hlee-sun         ###   ########.fr       */
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

static char	*get_path(t_Command **command, char **env_path, char *path)
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
		free_one((void **)&tmp);
		if (!path)
		{
			log_errors("Failed strjoin in get_path, path", "");
			return (NULL);
		}
		if (access(path, F_OK) == 0)
			break ;
		free_one((void **)&path);
		i++;
	}
	return (path);
}

int	find_and_check_path(t_Command **command, char **path)
{
	char	**env_path;

	if (ft_strrchr((*command)->cmd, '/') != NULL)
	{
		*path = ft_strdup((*command)->cmd);
		return (check_path(*path, command));
	}
	env_path = find_env(*((*command)->env));
	if (!env_path)
		return (cmd_error(command, ": No such file or directory\n", 127));
	*path = get_path(command, env_path, *path);
	all_free(&env_path);
	if (!(*path))
		return (cmd_error(command, ": command not found\n", 127));
	return (check_path(*path, command));
}
