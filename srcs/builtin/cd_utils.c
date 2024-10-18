/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 22:23:43 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/16 10:37:30 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_oldpwd(t_Command *command)
{
	char	*path;

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

static int	handle_home(t_Command *command)
{
	char	*path;
	char	*home;

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

int	check_and_set_path(t_Command *command)
{
	if (command->args[1][0] == '-' && command->args[1][1] == '\0')
	{
		return (handle_oldpwd(command));
	}
	else if (command->args[1][0] == '~' && (command->args[1][1] == '\0'
										|| command->args[1][1] == '/'))
	{
		return (handle_home(command));
	}
	return (SUCCESS);
}
