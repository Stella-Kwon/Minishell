/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:54 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 13:51:52 by hlee-sun         ###   ########.fr       */
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

// int	echo(t_Command *command)
// {
// 	int	n_option;

// 	n_option = false;
// 	if (command->args == NULL || command->args[1] == NULL)
// 	{
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 		command->exitcode = SUCCESS;
// 		return (command->exitcode);
// 	}
// 	if (is_n(command->args[1]) == true)
// 		n_option = true;
// 	print_args(command->args, 1 + n_option);
// 	if (!n_option)
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 	command->exitcode = SUCCESS;
// 	return (command->exitcode);
// }

int	echo(t_Command *command)
{
	int	n_option;
	size_t	arg_index;

	n_option = false;
	if (command->args == NULL || command->args[1] == NULL)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		command->exitcode = SUCCESS;
		return (command->exitcode);
	}

	// -n 옵션이 있는지 확인
	arg_index = 1;
	if (is_n(command->args[1]) == true)
	{
		n_option = true;
		arg_index = 2; // -n 옵션이 있으면 그 다음부터 출력
	}

	// args[arg_index]부터 출력
	print_args(command->args, arg_index);

	// -n 옵션이 없을 때만 개행 출력
	if (!n_option)
		ft_putchar_fd('\n', STDOUT_FILENO);

	command->exitcode = SUCCESS;
	return (command->exitcode);
}
