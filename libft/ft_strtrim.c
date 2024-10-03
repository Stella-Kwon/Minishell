/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 08:55:45 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/23 12:59:33 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*newstr;
	size_t	start;
	size_t	end;

	start = 0;
	end = ft_strlen(s1);
	if (!s1 || !set)
		return (0);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end && s1[end -1] && ft_strchr(set, s1[end - 1]))
		end--;
	if (start > end)
		return (ft_strdup(""));
	newstr = ft_substr(s1, start, end - start);
	return (newstr);
}
