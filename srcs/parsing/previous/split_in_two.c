/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_in_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:27:25 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/24 03:27:57 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char **split_tokens(char **initial_start_tokens, char *token, int count) // this one has to be free
{
    char **res;
    int i;

    i = 0;
    res = (char **)malloc(sizeof(char *) * (count + 1));
    if (!res)
    {
        log_errors("Failed to malloc res in split_tokens function");
        return (NULL);
    }
    while (*initial_start_tokens)
    {
        if (*initial_start_tokens == token)
            break;
        else
        {
            res[i] = ft_strdup(*initial_start_tokens);
            i++;
        }
        *initial_start_tokens++;
    }
    res[i] = '\0';
    return (res);
}