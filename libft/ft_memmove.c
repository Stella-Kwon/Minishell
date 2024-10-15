/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:07:53 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/23 12:57:58 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	unsigned char		*s;
	size_t				i;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (dst == src || n == 0)
		return (dst);
	if (dst < src)
	{
		i = -1;
		while (++i < n)
			d[i] = s[i];
	}
	else
	{
		i = n;
		while (i--)
			d[i] = s[i];
	}
	return (dst);
}
