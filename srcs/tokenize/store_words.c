/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/14 17:32:58 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *store_words(char **start)
{
    char *word_start;

    word_start = *start;
    while (**start && **start != ' ' && ft_isspace(**start) &&
           **start != '\'' && **start != '\"' && **start != '(' &&
           **start != ')' && **start != '|' && **start != '&' &&
           **start != '>' && **start != '$')
    {
        start++;
    }
    return (ft_strndup(word_start, start - word_start));
}