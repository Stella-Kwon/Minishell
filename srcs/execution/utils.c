/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:52:09 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 21:35:49 by hlee-sun         ###   ########.fr       */
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
	if (command == NULL || *command == NULL)
		return log_errors("cmd_error: Command is NULL", "");
	ft_putstr_fd("MINISHELL: ", STDERR_FILENO);
	ft_putstr_fd((*command)->cmd, STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	(*command)->exitcode = error_nb;
	return (FAIL);
}

int	check_path(char *path, t_Command **command)
{
	if (path == NULL || command == NULL || *command == NULL)
        return (log_errors("check_path: NULL argument", ""));
	if (access(path, F_OK) == -1)
		return (cmd_error(command, ": command not found\n", 127));
	if (access(path, X_OK) == -1)
		return (cmd_error(command, ": Permission denied\n", 126));
	return (SUCCESS);
}

int	check_cmd_script(t_Command **command)
{
	int	len;

	if (command == NULL || *command == NULL)
        return (log_errors("check_cmd_script: Command is NULL", ""));
	len = ft_strlen((*command)->cmd);
	if (len > 3 && (*command)->cmd[len - 3] == '.' && \
		(*command)->cmd[len - 2] == 's' && (*command)->cmd[len - 1] == 'h')
	{
		if (ft_strrchr((*command)->cmd, '/') == NULL)
		{
			cmd_error(command, ": command not found\n", 127);
			return (FAIL);
		}
		check_path((*command)->cmd, command);
	}
	return (SUCCESS);
}

// int	check_cmd_error(t_Command **command)
// {
// 	int	len;

// 	if ((*command)->cmd == NULL)
// 		return (cmd_error(command, ": Command not found\n", 127));
// 	if ((*command)->cmd[0] == '.' || (*command)->cmd[0] == '/')
// 	{
// 		len = ft_strlen((*command)->cmd);
// 		if ((*command)->cmd[len - 1] == '/')
// 		{
// 			ft_putstr_fd("MINISHELL: ", STDERR_FILENO);
// 			ft_putstr_fd((*command)->cmd, STDERR_FILENO);
// 			return (error_exitcode(command, " is a directory", 126));
// 		}
// 		if (access((*command)->cmd, F_OK) == -1)
// 			return (cmd_error(command, ": No such file or directory\n", 127));
// 		if (access((*command)->cmd, X_OK) == -1)
// 			return (cmd_error(command, ": Permission denied\n", 126));
// 	}
// 	return (SUCCESS);
// }

int	check_cmd_error(t_Command **command)
{
    int	len;
	    // NULL 체크
    if (command == NULL || *command == NULL)
        return log_errors("check_cmd_error: Command is NULL", "");
    if ((*command)->cmd == NULL)
        return (cmd_error(command, ": Command not found\n", 127));
    if ((*command)->cmd[0] == '.' || (*command)->cmd[0] == '/')
    {
        len = ft_strlen((*command)->cmd);
        if ((*command)->cmd[len - 1] == '/')
        {
            return (cmd_error(command, ": Is a directory\n", 126));
        }
        if (access((*command)->cmd, F_OK) == -1)
            return (cmd_error(command, ": No such file or directory\n", 127));
        if (access((*command)->cmd, X_OK) == -1)
            return (cmd_error(command, ": Permission denied\n", 126));
    }
    return (SUCCESS);
}
