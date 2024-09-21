/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:26:48 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 20:41:02 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **ft_realloc(char **token, int position, int *bufsize)
{
    char **realloc_token;

    if (position >= *bufsize)
    {
        *bufsize *= 2;
        realloc_token = (char **)malloc(*bufsize * sizeof(char *));
        if (!realloc_token)
            return (NULL);
        memcpy(realloc_token, token, position * sizeof(char *));
        if (token)
            free_one(token);
        token = realloc_token;
    }
    return (token);
}
