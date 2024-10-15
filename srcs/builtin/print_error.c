/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:27 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 02:00:51 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	ft_perror(char *message)
// {
// 	ft_putstr_fd("minisehll: ", STDERR_FILENO);
// 	ft_putstr_fd(message, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	ft_putstr_fd(strerror(errno), STDERR_FILENO);
// 	ft_putstr_fd("\n", STDERR_FILENO);
// }

int	print_error_cd(t_Command *command, char *s, int error_nb)
{
	ft_putstr_fd("minisehll: cd: ", STDERR_FILENO);
	ft_putstr_fd(command->args[1], STDERR_FILENO);
	return (error_exitcode(&command, s, error_nb));
}
