/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:36:28 by hlee-sun          #+#    #+#             */
/*   Updated: 2023/11/23 12:57:20 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_size(int n)
{
	size_t	size;

	size = 0;
	if (n < 0)
		size++;
	else if (n == 0)
		size++;
	while (n != 0)
	{
		n = n / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	size_t	size;
	char	*str;
	long	nb;

	size = ft_size(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	nb = (long)n;
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		str[0] = '0';
	str[size--] = '\0';
	while (nb != 0)
	{
		str[size--] = nb % 10 + '0';
		nb /= 10;
	}
	return (str);
}
