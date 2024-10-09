/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:26:48 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/07 23:19:17 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_realloc(char **token, int position, int *bufsize)
{
	char	**realloc_token;
	int		i;

	i = 0;
	if (position >= *bufsize)
	{
		*bufsize *= 2;
		realloc_token = (char **)malloc(*bufsize * sizeof(char *));
		if (!realloc_token)
			return (NULL);
		if (token)
		{
			while (i < position)
			{
				realloc_token[i] = token[i];
				i++;
			}
			free_one((void **)&token);
			token = realloc_token;
		}
	}
	return (token);
}
