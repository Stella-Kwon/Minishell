/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:34 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *handle_quote_in_word(t_For_tokenize *tokenize, char *start, char ref)
{
	char *prefix;
	char *combined;

	if (tokenize->start > start)
	{
		prefix = ft_strndup(start, tokenize->start - start);
		// printf("tokenize->tokens[%d]: %s\n", tokenize->token_count, tokenize->tokens[tokenize->token_count]);
		// printf("prefix : %s\n", prefix);
		if (!prefix)
			return (NULL);
		handle_set(tokenize, ref); 
		// printf("tokenize->tokens[%d]: %s\n", tokenize->token_count, tokenize->tokens[tokenize->token_count]);
		tokenize->token_count--;
		// printf("tokenize->tokens[%d]: %s\n", tokenize->token_count, tokenize->tokens[tokenize->token_count]);
		combined = ft_strjoin(prefix, tokenize->tokens[tokenize->token_count]);
		// printf("combined : %s\n", combined);
		free(prefix);
		free(tokenize->tokens[tokenize->token_count]);
		return (combined);
	}
	else
	{
		handle_set(tokenize, ref);
		tokenize->token_count--;
		return (tokenize->tokens[tokenize->token_count]);
	}
}

char *store_words(t_For_tokenize *tokenize)
{
	char *start;
	char *result;

	start = tokenize->start;
	while (*tokenize->start && !ft_isspace(*tokenize->start) &&
		   *tokenize->start != '|' && *tokenize->start != '&' &&
		   *tokenize->start != '>' && *tokenize->start != '<')
	{
		if (*tokenize->start == '\'')
			return (handle_quote_in_word(tokenize, start, '\''));
		else if (*tokenize->start == '"')
			return (handle_quote_in_word(tokenize, start, '"'));
		else if (*tokenize->start == '(')
			return (handle_quote_in_word(tokenize, start, '('));
		else
			(tokenize->start)++;
	}
	result = ft_strndup(start, tokenize->start - start);
	if (!result)
	{
		log_errors("Failed to store word", "");
		return (NULL);
	}
	return (result);
}

int store_str(t_For_tokenize *tokenize)
{
	if (!ft_isspace(*tokenize->start) && *tokenize->start != '\0')
	{
		tokenize->tokens[tokenize->token_count] = store_words(tokenize);
		if (!tokenize->tokens[tokenize->token_count])
		{
			all_free(&tokenize->tokens);
			return (FAIL);
		}
		tokenize->token_count++;
		tokenize->tokens = ft_realloc_double(tokenize->tokens,
											 tokenize->token_count, &tokenize->buffsize);
		if (!tokenize->tokens)
			return (log_errors("Failed to \"reallocate\" memory for tokens",
							   ""));
	}
	return (SUCCESS);
}
