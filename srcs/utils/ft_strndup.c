/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:28:05 by skwon2            #+#    #+#             */
/*   Updated: 2024/09/13 22:53:41 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(char *str, int n)
{
	char	*res;
	int		i;

	res = (char *)malloc(n + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
