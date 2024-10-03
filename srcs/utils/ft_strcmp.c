/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 12:53:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/18 16:53:59 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int ft_strcmp(const char *line, const char *limiter)
{
    size_t i;
    size_t n;

    i = 0;
    n = ft_strlen(limiter);
    if ((ft_strlen(line)) != n)
        return (-1);
    while (i < n && (line[i] != '\0' || limiter[i] != '\0'))
    {
        if ((unsigned char)limiter[i] != (unsigned char)line[i])
            return (-1);
        i++;
    }
    return (0);
}
