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

static int	handle_exit_arguments(t_Command *command, size_t *i)
{
	if (!command->args[1])
	{
		command->exitcode = 0;
		exit(command->exitcode);
	}
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
			all_free(command->env);
			exit(2);
		}
		i++;
	}
	exit_status = ft_atoi(command->args[1]);
	command->exitcode = exit_status % 256;
	if (command->exitcode < 0)
		command->exitcode += 256;
	free(*(command->env));
	exit(command->exitcode);
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
