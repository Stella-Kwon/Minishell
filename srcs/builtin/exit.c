/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:15 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/12 18:38:09 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_exit_err_msg(char *arg)
{
	ft_putstr_fd("minishell, exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
	exit (2);
}

int	mini_exit(t_Command *command)
{
	int		exit_status;
	size_t	i;

	i = 0;
	ft_putstr_fd("exit\n", 2);
	if (command->args[1] == NULL)
	{
		command->exitcode = 0;
		exit(command->exitcode);
	}
	if (command->args[2] != NULL)
	{
		ft_putstr_fd("minisehell:exit:too many arguments\n", STDERR_FILENO);
		command->exitcode = 1;
		return (command->exitcode);
	}
	while (command->args[1][i] != '\0')
	{
		if (command->args[1][i] == '-' || command->args[1][i] == '+')
			i++;
		if (!ft_isdigit(command->args[1][i]))
			print_exit_err_msg(command->args[1]);
		i++;
	}
	exit_status = ft_atoi(command->args[1]);
	command->exitcode = exit_status % 256;
	if (command->exitcode < 0)
		command->exitcode += 256;
	exit(command->exitcode);
}
