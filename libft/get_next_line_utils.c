/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 14:19:56 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/03/15 13:42:41 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*check_string(char *str)
{
	if (!str)
	{
		str = (char *)malloc(sizeof(char) * 1);
		if (!str)
			return (0);
		*str = 0;
	}
	return (str);
}

char	*ft_free(char **str)
{
	free(*str);
	*str = 0;
	return (0);
}
