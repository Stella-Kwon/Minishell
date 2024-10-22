/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 01:07:03 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/22 22:07:06 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**init_args_new(void)
{
	char	**args_new;

	args_new = ft_calloc(3, sizeof(*args_new));
	if (!args_new)
	{
		log_errors("Failed calloc in cd", "");
		return (NULL);
	}
	args_new[0] = ft_strdup("export");
	if (!args_new[0])
	{
		log_errors("Failed strdup in cd", "");
		free(args_new);
		return (NULL);
	}
	return (args_new);
}

static void	export_paths_and_free(t_Command *command, char **args_new, \
									char *path_old, char *path_new)
{
	char	**args_old;

	args_new[1] = path_old;
	args_old = command->args;
	command->args = args_new;
	export(command);
	free(args_new[1]);
	args_new[1] = path_new;
	export(command);
	command->args = args_old;
	free(args_new[0]);
	free(args_new[1]);
	free(args_new);
}

static void	go_home(t_Command *command)
{
	char	*path_old;
	char	*path_new;
	char	**args_new;

	path_old = get_current_dir("OLDPWD=");
	if (!path_old)
		return ;
	if (chdir(get_env_value("HOME", *(command->env))) == SUCCESS)
	{
		args_new = init_args_new();
		if (!args_new)
			return ;
		path_new = ft_strjoin("PWD=", get_env_value("HOME", *(command->env)));
		if (!path_new)
		{
			log_errors("Failed strjoin in cd, go home", "");
			free(path_old);
			free(args_new[0]);
			free(args_new);
			return ;
		}
		export_paths_and_free(command, args_new, path_old, path_new);
	}
}

static int	change_directory(t_Command *command)
{
	char	*new_dir;

	if (chdir(command->args[1]) == SUCCESS)
	{
		new_dir = get_current_dir("PWD=");
		if (!new_dir)
			return (FAIL);
		else
		{
			free(command->args[1]);
			command->args[1] = new_dir;
			export(command);
		}
	}
	else
	{
		if (errno == ENOTDIR)
			print_error_cd(command, ": Not a directory\n", 1);
		else if (errno == ENOENT)
			print_error_cd(command, ": No such file or directory\n", 1);
		else
			cmd_error(&command, ": cd failed\n", 1);
		return (FAIL);
	}
	return (SUCCESS);
}

int	cd(t_Command *command)
{
	if (command->args[1] == NULL || command->args[1][0] == '\0')
		go_home(command);
	else if (command->args[2] != NULL)
		return (cmd_error(&command, ": too many arguments\n", 1));
	else
	{
		if (command->args[1][0] == '-' || command->args[1][0] == '~')
		{
			if (check_and_set_path(command) == FAIL)
				return (FAIL);
		}
		currdir_to_old_pwd(command);
		if (change_directory(command) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
