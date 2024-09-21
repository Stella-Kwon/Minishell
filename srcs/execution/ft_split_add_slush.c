/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_add_slush.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:20:08 by suminkwon         #+#    #+#             */
/*   Updated: 2024/04/12 14:42:47 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	count_words(char *str, char c)
{
	size_t	i;

	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			i++;
			while (*str && *str != c)
				str++;
		}
		else
			str++;
	}
	return (i);
}

static size_t	get_each_strlen(char **str, char c)
{
	size_t	i;

	i = 0;
	while (**str && **str == c)
		(*str)++;
	while ((*str)[i])
	{
		if ((*str)[i] == c)
			break ;
		i++;
	}
	return (i + 1);
}

static char	*loop_in_get_res(size_t	*each_strlen, char *res, char **str, char c)
{
	*each_strlen = get_each_strlen(str, c);
	res = (char *)malloc((*each_strlen + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, *str, *each_strlen);
	ft_strlcat(res + (*each_strlen -1), "/", *each_strlen + 1);
	return (res);
}

static char	**get_res(char *str, char c)
{
	char	**res;
	size_t	each_strlen;
	size_t	i;
	size_t	res_len;

	res_len = count_words(str, c);
	res = (char **)malloc((res_len + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	each_strlen = 0;
	while (i < res_len)
	{
		res[i] = loop_in_get_res(&each_strlen, res[i], &str, c);
		if (!res[i])
			return (all_free(&res));
		str += each_strlen -1;
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split_add_slush(char const *s, char c)
{
	char	**res;
	char	*str;

	if (!s)
		return (NULL);
	str = (char *)s;
	res = get_res(str, c);
	free(str);
	str = NULL;
	return (res);
}
