/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 01:07:03 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 02:36:55 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_and_set_path(t_Command *command)
{
	char	*path;
	char	*home;

	home = NULL;
	path = NULL;
	if (command->args[1][0] == '-' && command->args[1][1] == '\0')
	{
		path = get_env_value("OLDPWD", command->env);
		if (path == NULL)
		{
			print_error_cd(command, ": OLDPWD not set\n", 1);
			return ;
		}
		command->args[1] = ft_strdup(path);
		if (command->args[1] == NULL)
		{
			print_error_cd(command, ": strdup failed\n", 1);
			return ;
		}
	}
	else if (command->args[1][0] == '~' && (command->args[1][1] == '\0' || \
			command->args[1][1] == '/'))
	{
		path = get_env_value("HOME", command->env);
		if (path == NULL)
		{
			print_error_cd(command, ": HOME not set\n", 1);
			return ;
		}
		home = ft_strjoin(path, command->args[1] + 1);
		if (home == NULL)
		{
			print_error_cd(command, ": strjoin failed\n", 1);
			return ;
		}
		command->args[1] = home;
	}
}

static void	currdir_to_old_pwd(t_Command *command)
{
	char	*here;
	char	*path;
	char	*tmp;

	here = ft_calloc(1, MAXPATHLEN);
	if (here == NULL)
	{
		print_error_cd(command, ": calloc failed\n", 1);
		return ;
	}
	if (getcwd(here, MAXPATHLEN) == NULL)
	{
		print_error_cd(command, ": getcwd failed\n", 1);
		free(here);
		return ;
	}
	path = ft_strjoin("OLDPWD=", here);
	free(here);
	if (path == NULL)
	{
		print_error_cd(command, ": strjoin failed\n", 1);
		return ;
	}
	tmp = command->args[1];
	command->args[1] = path;
	export(command);
	free(path);
	command->args[1] = tmp;
}

static void	go_home(t_Command *command)
{
	char	*path_old;
	char	*path_new;
	char	**args_new;

	if (chdir(get_env_value("HOME", command->env)) == SUCCESS)
	{
		args_new = ft_calloc(3, sizeof(*args_new));
		if (args_new == NULL)
		{
			print_error_cd(command, ": calloc failed\n", 1);
			return ;
		}
		args_new[0] = ft_strdup("export");
		if (args_new[0] == NULL)
		{
			print_error_cd(command, ": strdup failed\n", 1);
			free(args_new);
			return ;
		}
		path_old = ft_strjoin("PWD=", get_env_value("HOME", command->env));
		if (path_old == NULL)
		{
			print_error_cd(command, ": strjoin failed\n", 1);
			free(args_new[0]);
			free(args_new);
			return ;
		}
		path_new = ft_strjoin("OLDPWD=", get_env_value("HOME", command->env));
		if (path_new == NULL)
		{
			print_error_cd(command, ": strjoin failed\n", 1);
			free(path_old);
			free(args_new[0]);
			free(args_new);
			return ;
		}
		args_new[1] = path_old;
		export(command);
		free(args_new[1]);
		args_new[1] = path_new;
		export(command);
		free(args_new[0]);
		free(args_new[1]);
		free(args_new);
	}
}

static char	*get_current_dir(void)
{
	char	*path;
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (buf == NULL)
	{
		perror("Error: calloc failed\n");
		return (NULL);
	}
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		perror("Error: getcwd failed\n");
		free(buf);
		return (NULL);
	}
	path = ft_strjoin("PWD=", buf);
	free(buf);
	if (path == NULL)
	{
		perror("Error: strjoin failed\n");
		return (NULL);
	}
	return (path);
}

int	cd(t_Command *command)
{
	int		status;
	char 	*new_dir;

	status = SUCCESS;
	if (command->args[1] == NULL)
		go_home(command);
	else
	{
		if (command->args[1][0] == '-' || command->args[1][0] == '~')
			check_and_set_path(command);
		currdir_to_old_pwd(command);
		if (chdir(command->args[1]) == SUCCESS)
		{
			new_dir = get_current_dir();
			if (new_dir == NULL)
				status = FAIL;
			else
			{
				command->args[1] = new_dir;
				export(command);
			}
		}
		else
		{
			print_error_cd(command, ": No such file or directory\n", 1);
			status = FAIL;
		}
	}
	return (status);
}
