/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:52:09 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 21:59:31 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_exitcode(t_Command **command, char *s, int error_nb)
{
	ft_putstr_fd(s, STDERR_FILENO);
	(*command)->exitcode = error_nb;
	return (FAIL);
}

int	cmd_error(t_Command **command, char *s, int error_nb)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((*command)->cmd, STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	(*command)->exitcode = error_nb;
	return (FAIL);
}

int	check_path(char *path, t_Command **command)
{
	if (access(path, F_OK) == -1)
		return (cmd_error(command, ": command not found\n", 127));
	if (access(path, X_OK) == -1)
		return (cmd_error(command, ": Permission denied\n", 126));
	return (SUCCESS);
}

int	check_cmd_script(t_Command **command)
{
	int	len;

	len = ft_strlen((*command)->cmd);
	if (len > 3 && (*command)->cmd[len - 3] == '.' && \
		(*command)->cmd[len - 2] == 's' && (*command)->cmd[len - 1] == 'h')
	{
		if (ft_strrchr((*command)->cmd, '/') == NULL)
		{
			cmd_error(command, ": command not found\n", 127);
			return (FAIL);
		}
		if (check_path((*command)->cmd, command) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

int	check_cmd_error(t_Command **command)
{
	int	len;

	len = ft_strlen((*command)->cmd);
	if ((*command)->cmd[0] == '.' || (*command)->cmd[0] == '/')
	{
		if ((*command)->cmd[len - 1] == '/')
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((*command)->cmd, STDERR_FILENO);
			return (error_exitcode(command, ": Is a directory", 126));
		}
		if (access((*command)->cmd, F_OK) == -1)
			return (cmd_error(command, ": No such file or directory\n", 127));
		if (access((*command)->cmd, X_OK) == -1)
			return (cmd_error(command, ": Permission denied\n", 126));
	}
	return (SUCCESS);
}
