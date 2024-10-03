/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 06:26:02 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/03/15 13:45:55 by hlee-sun         ###   ########.fr       */
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
