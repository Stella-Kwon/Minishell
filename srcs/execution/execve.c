/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 14:06:18 by suminkwon         #+#    #+#             */
/*   Updated: 2024/04/24 12:18:37 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	split_cmd_error(char ***split_cmd)
{
	if (!(*split_cmd))
		return (FAILED);
	if ((*split_cmd)[0][0] == '.')
	{
		if (access((*split_cmd)[0], X_OK) == -1)
		{
			errors("bash: ", 0, NULL, (*split_cmd)[0]);
			all_free(split_cmd);
			if (access("filename", F_OK) == -1)
				return (126);
			return (127);
		}
		else
			errors("bash", 1, (*split_cmd)[0], "is a directory");
		all_free(split_cmd);
		return (126);
	}
	if ((*split_cmd)[0][0] == '/' && access((*split_cmd)[0], X_OK) == -1)
	{
		errors("bash", 1, (*split_cmd)[0], "No such file or directory");
		all_free(split_cmd);
		return (127);
	}
	return (SUCCESS);
}

static	int	empty_space_check(char *argv, char **path)
{
	if (count_words(argv, ' ') == 0)
	{
		if (!(*path))
			errors("bash", 1, argv, "No such file or directory");
		else
			errors("bash", 1, argv, "command not found");
		return (127);
	}
	return (SUCCESS);
}

int	top_execution(char *argv, char **env, char **path, char ***split_cmd)
{
	int	exitcode;

	if (argv[0] == '\0')
	{
		errors("bash", 1, argv, "command not found");
		return (127);
	}
	*path = find_path(env);
	if (empty_space_check(argv, path) != SUCCESS)
		return (127);
	*split_cmd = ft_split_several(argv, ' ', '"', '\'');
	exitcode = split_cmd_error(split_cmd);
	if (exitcode != SUCCESS)
		return (exitcode);
	if (!(*path) && access((*split_cmd)[0], X_OK) == -1)
	{
		errors("bash", 1, (*split_cmd)[0], "No such file or directory");
		all_free(split_cmd);
		return (127);
	}
	return (SUCCESS);
}

int	execution(char *argv, char **env)
{
	char	*path;
	char	*cmd_path;
	char	**split_cmd;
	int		exitcode;

	split_cmd = NULL;
	cmd_path = NULL;
	path = NULL;
	exitcode = top_execution(argv, env, &path, &split_cmd);
	if (exitcode != SUCCESS)
		return (exitcode);
	cmd_path = find_right_path(argv, &path, &split_cmd);
	if (execve(cmd_path, split_cmd, (char *const *)env) == -1)
	{
		errors("bash", 1, (split_cmd)[0], "command not found");
		if (split_cmd)
			all_free(&split_cmd);
		free_one(&cmd_path);
	}
	return (127);
}
