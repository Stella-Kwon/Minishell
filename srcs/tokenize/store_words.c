/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/28 23:28:20 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *store_words(char **start)
{
    char *word_start;
    
    // printf("store : %c\n", **start);
    word_start = *start;
    while (**start && !ft_isspace(**start) &&
            **start != '\'' && **start != '\"' && **start != '(' &&
            **start != ')' && **start != '|' && **start != '&' &&
            **start != '>' && **start != '$')
    {
        (*start)++;
        // printf("c : %c\n", **start);
    }

    return (ft_strndup(word_start, *start - word_start));
}
