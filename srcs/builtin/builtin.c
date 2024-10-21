/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:38 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 00:50:47 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_builtin(t_Command *command)
{
	if (ft_strncmp(command->cmd, "cd", 3) == 0 || \
		ft_strncmp(command->cmd, "export", 7) == 0 || \
		ft_strncmp(command->cmd, "unset", 6) == 0 || \
		ft_strncmp(command->cmd, "exit", 5) == 0 || \
		ft_strncmp(command->cmd, "echo", 5) == 0 || \
		ft_strncmp(command->cmd, "pwd", 4) == 0 || \
		ft_strncmp(command->cmd, "env", 4) == 0)
		return (TRUE);
	return (FALSE);
}

int	builtin(t_Command *command)
{
	command->exitcode = 0;
	if (ft_strncmp(command->cmd, "cd", 3) == 0)
		return (cd(command));
	else if (ft_strncmp(command->cmd, "export", 7) == 0)
		return (export(command));
	else if (ft_strncmp(command->cmd, "unset", 6) == 0)
		return (unset(command));
	else if (ft_strncmp(command->cmd, "exit", 5) == 0)
		return (mini_exit(command));
	else if (ft_strncmp(command->cmd, "echo", 5) == 0)
		return (echo(command));
	else if (ft_strncmp(command->cmd, "pwd", 4) == 0)
		return (pwd(command));
	else if (ft_strncmp(command->cmd, "env", 4) == 0)
		return (env(command));
	return (FAIL);
}
