/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:34 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*store_words(char **start)
{
	char	*word_start;

	word_start = *start;
	while (**start && !ft_isspace(**start) && **start != '(' && \
			**start != ')' && **start != '|' && **start != '&' && \
			**start != '>' && **start != '<')
	{
		(*start)++;
	}
	return (ft_strndup(word_start, *start - word_start));
}

int	store_str(t_For_tokenize *tokenize, int *buffsize)
{
	if (!ft_isspace(*tokenize->start) && *tokenize->start != '\0')
	{
		tokenize->tokens[tokenize->token_count] = \
		store_words(&tokenize->start);
		if (!tokenize->tokens[tokenize->token_count])
		{
			all_free(&tokenize->tokens);
			return (log_errors("Failed to store word", ""));
		}
		tokenize->token_count++;
		tokenize->tokens = ft_realloc_double(tokenize->tokens, \
		tokenize->token_count, buffsize);
		if (!tokenize->tokens)
			return (log_errors("Failed to \"reallocate\" \
			memory for tokens", ""));
	}
	return (SUCCESS);
}

int	is_special_character(char c)
{
	if (c == '\\' || c == ';' || c == '^' || c == '#' || \
		c == '@' || c == '!' || c == '*' || c == '%' || c == '(' || c == ')')
		return (TRUE);
	return (FALSE);
}

int	handle_whitespace(t_For_tokenize *tokenize)
{
	while (ft_isspace(*tokenize->start))
		(tokenize->start)++;
	return (SUCCESS);
}
