/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:54 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 04:35:42 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_n(char *str)
{
	size_t	i;

	if (str[0] == '-' && str[1] == 'n')
	{
		i = 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}

static void	print_args(char **args, size_t i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	echo(t_Command *command)
{
	int		n_option;
	size_t	arg_index;

	n_option = false;
	if (command->args == NULL || command->args[1] == NULL)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		command->exitcode = SUCCESS;
		return (command->exitcode);
	}
	arg_index = 1;
	if (is_n(command->args[1]) == true)
	{
		n_option = true;
		arg_index = 2;
	}
	print_args(command->args, arg_index);
	if (!n_option)
		ft_putchar_fd('\n', STDOUT_FILENO);

	command->exitcode = SUCCESS;
	return (command->exitcode);
}
