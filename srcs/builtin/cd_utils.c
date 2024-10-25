/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 22:23:43 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/24 16:14:09 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_oldpwd(t_Command *command)
{
	char *path;

	path = get_env_value("OLDPWD", *(command->env));
	if (!path)
	{
		cmd_error(&command, ": OLDPWD not set\n", 1);
		return (FAIL);
	}
	free(command->args[1]);
	command->args[1] = ft_strdup(path);
	if (!command->args[1])
	{
		cmd_error(&command, ": strdup failed\n", 1);
		return (FAIL);
	}
	ft_putstr_fd(command->args[1], STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}

static int handle_home(t_Command *command)
{
	char *path;
	char *home;

	path = get_env_value("HOME", *(command->env));
	home = ft_strjoin(path, command->args[1] + 1);
	if (!path)
	{
		cmd_error(&command, ": HOME not set\n", 1);
		return (FAIL);
	}
	if (!home)
	{
		cmd_error(&command, ": strjoin failed\n", 1);
		return (FAIL);
	}
	free(command->args[1]);
	command->args[1] = home;
	return (SUCCESS);
}

int check_and_set_path(t_Command *command)
{
	if (command->args[1][0] == '-' && command->args[1][1] == '\0')
	{
		return (handle_oldpwd(command));
	}
	else if (command->args[1][0] == '~' && (command->args[1][1] == '\0' || command->args[1][1] == '/'))
	{
		return (handle_home(command));
	}
	return (SUCCESS);
}

void currdir_to_old_pwd(t_Command *command)
{
	char *path;
	char *tmp;

	path = get_current_dir("OLDPWD=");
	if (!path)
		return;
	tmp = command->args[1];
	command->args[1] = path;
	export(command);
	command->args[1] = tmp;
	free(path);
}

char *get_current_dir(char *prefix)
{
	char *path;
	char *buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (!buf)
	{
		log_errors("Failed calloc in cd utils2", "");
		return (NULL);
	}
	if (!getcwd(buf, MAXPATHLEN))
	{
		free(buf);
		return (NULL);
	}
	path = ft_strjoin(prefix, buf);
	free(buf);
	if (!path)
	{
		log_errors("Failed strjoin in cd utils2", "");
		return (NULL);
	}
	return (path);
}