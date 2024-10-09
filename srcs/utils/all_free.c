/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:05:27 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/07 23:15:33 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	all_free(char ***res)
{
	int	i;

	i = 0;
	if (!res || !**res || !*res)
		return ;
	while ((*res)[i])
	{
		free((*res)[i]);
		(*res)[i] = NULL;
		i++;
	}
	free(*res);
	*res = NULL;
}
