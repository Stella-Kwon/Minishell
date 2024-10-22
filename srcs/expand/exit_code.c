/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:55:10 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/14 04:56:05 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	for_exit_code(t_Dollar *dol, int last_exitcode)
{
	char	*exit_code_str;

	exit_code_str = ft_itoa(last_exitcode);
	if (!exit_code_str)
		return (FAIL);
	dol->var_value = exit_code_str;
	dol->i++;
	return (SUCCESS);
}
