/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:15 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/16 10:37:45 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_and_exit(t_Command *command, int exitcode)
{
	all_free(command->env);
	free_astnode(command->root_node);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exitcode);
}

static int	handle_exit_arguments(t_Command *command, size_t *i)
{
	if (!command->args[1])
		free_and_exit(command, 0);
	if (command->args[2] != NULL)
	{
		cmd_error(&command, ": too many arguments\n", 1);
		return (command->exitcode);
	}
	if (command->args[1][*i] == '-' || command->args[1][*i] == '+')
		(*i)++;
	return (SUCCESS);
}

static void	validate_exit_status(t_Command *command, size_t i)
{
	int	exit_status;

	while (command->args[1][i] != '\0')
	{
		if (!ft_isdigit(command->args[1][i]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(command->args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free_and_exit(command, 2);
		}
		i++;
	}
	exit_status = ft_atoi(command->args[1]);
	exit_status = exit_status % 256;
	if (exit_status < 0)
		exit_status += 256;
	free_and_exit(command, exit_status);
}

int	mini_exit(t_Command *command)
{
	size_t	i;

	i = 0;
	if (handle_exit_arguments(command, &i) != SUCCESS)
		return (command->exitcode);
	validate_exit_status(command, i);
	return (command->exitcode);
}
