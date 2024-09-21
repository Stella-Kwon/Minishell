/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_direct_strndup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:28:05 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/13 22:53:41 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char    *ft_strndup(char *str, int n)
{
    char *res;
    int i;

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