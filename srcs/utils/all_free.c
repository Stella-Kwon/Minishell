/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:05:27 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/25 18:45:50 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	all_free(char ***res)
{
	int	i;

	i = 0;
	if (*res && **res)
	{
		while ((*res)[i])
		{
			free((*res)[i]);
			(*res)[i] = NULL;
			i++;
		}
		free(*res);
		*res = NULL;
	}
	if (*res)
	{
		free(*res);
		*res = NULL;
	}
}
