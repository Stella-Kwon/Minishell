/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:55:55 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/23 12:57:11 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*dst;

	if (count > 0 && SIZE_MAX / count < size)
		return (NULL);
	dst = (void *)malloc(count * size);
	if (!dst)
		return (0);
	ft_bzero(dst, (count * size));
	return (dst);
}
