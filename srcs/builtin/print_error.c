/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:27 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/14 04:49:14 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_error_cd(t_Command *command, char *s, int error_nb)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(command->args[1], STDERR_FILENO);
	return (error_exitcode(&command, s, error_nb));
}
