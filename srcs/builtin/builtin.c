/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:38 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 02:27:17 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_filesystem(t_Command *command)
{
	command->exitcode = 0;
	if (ft_strncmp(command->cmd, "cd", 3) == 0)
		cd(command);
	else if (ft_strncmp(command->cmd, "export", 7) == 0)
		export(command);
	else if (ft_strncmp(command->cmd, "unset", 6) == 0)
		unset(command);
	else if (ft_strncmp(command->cmd, "exit", 5) == 0)
		mini_exit(command);
	else
		return (FAIL);
	return (SUCCESS);
}

int	builtin_with_output(t_Command *command)
{
	command->exitcode = 0;
	if (ft_strncmp(command->cmd, "echo", 5) == 0)
		echo(command);
	else if (ft_strncmp(command->cmd, "pwd", 4) == 0)
		pwd(command);
	else if (ft_strncmp(command->cmd, "env", 4) == 0)
		env(command);
	else
		return (FAIL);
	return (SUCCESS);
}
