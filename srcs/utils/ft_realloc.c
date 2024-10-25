/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:26:48 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:46:01 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_realloc_double(char **token, int position, int *bufsize)
{
	char	**realloc_token;
	int		i;

	i = 0;
	if (position >= *bufsize)
	{
		*bufsize *= 2;
		realloc_token = (char **)ft_calloc(*bufsize, sizeof(char *));
		if (!realloc_token)
			return (NULL);
		if (token)
		{
			while (i < position)
			{
				realloc_token[i] = token[i];
				i++;
			}
			free(token);
			token = NULL;
			token = realloc_token;
		}
	}
	return (token);
}

char	*ft_realloc_single(char *token, int position, int *bufsize)
{
	char	*realloc_token;
	int		i;

	i = 0;
	if (position >= *bufsize)
	{
		*bufsize *= 2;
		realloc_token = (char *)ft_calloc(*bufsize, sizeof(char));
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
