/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:38 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/09 17:30:40 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_filesystem(t_Command *command)
{
	if (ft_strncmp(command->cmd, "cd", 3) == 0)
		return (cd(command->args, &command->env));
	else if (ft_strncmp(command->cmd, "export", 7) == 0)
		return (export(command->args, &command->env));
	else if (ft_strncmp(command->cmd, "unset", 6) == 0)
		return (unset(command->args, &command->env));
	else if (ft_strncmp(command->cmd, "exit", 5) == 0)
		return (mini_exit(command->args, &command->env));
	return (FAIL);
}

int	builtin_with_output(t_Command *command)
{
	if (ft_strncmp(command->cmd, "echo", 5) == 0)
		return (echo(command->args, &command->env));
	else if (ft_strncmp(command->cmd, "pwd", 4) == 0)
		return (pwd(command->args, &command->env));
	else if (ft_strncmp(command->cmd, "env", 4) == 0)
		return (env(command->args, &command->env));
	return (FAIL);
}
