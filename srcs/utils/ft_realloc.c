/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:26:48 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/24 23:41:52 by skwon2           ###   ########.fr       */
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
			// all_free(&token);
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
