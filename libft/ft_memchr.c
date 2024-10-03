/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:29:57 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/23 12:57:48 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *b, int c, size_t n)
{
	size_t			i;
	unsigned char	*new_b;
	unsigned char	find;

	new_b = (unsigned char *)b;
	find = c;
	i = 0;
	while (i < n)
	{
		if (new_b[i] == find)
			return ((new_b + i));
		i++;
	}
	return (0);
}
