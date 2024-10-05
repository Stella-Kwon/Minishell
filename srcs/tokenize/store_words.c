/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 00:36:03 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*store_words(char **start)
{
	char	*word_start;

	word_start = *start;
	while (**start && !ft_isspace(**start) && **start != '\'' && \
			**start != '\"' && **start != '(' && **start != ')' && \
			**start != '|' && **start != '&' && **start != '>' && \
			**start != '$')
	{
		(*start)++;
	}
	return (ft_strndup(word_start, *start - word_start));
}
