/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:09:30 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 17:13:59 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_pipe_and_or(t_For_tokenize *tokenize)
{
	int len;
	int i;

	i = 0;
	if (*(tokenize->start + 1) == '|')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(tokenize->start + len + i)))
		i++;
	if (*(tokenize->start + len + i) == '\0')
	{
		i = readline_again_after_operator(tokenize);
		if (i != SUCCESS)
			return (i);
	}
	else
	{
		// error_handling_in_middle_operator(tokenize, len, &i);
		if (redirect_operation_error(tokenize->start + len + i - 1) != SUCCESS)
			return (2);
	}
	return (handle_token(tokenize, len));
}

// int	handle_and_and_background(t_For_tokenize *tokenize)
int handle_and(t_For_tokenize *tokenize)
{
	int len;
	int i;

	i = 0;
	if (*(tokenize->start + 1) == '&')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(tokenize->start + len + i)))
		i++;
	if (*(tokenize->start + len + i) == '\0')
	{
		i = readline_again_after_operator(tokenize);
		if (i != SUCCESS)
			return (i);
	}
	else
	{
		// error_handling_in_middle_operator(tokenize, len, &i);
		if (redirect_operation_error(tokenize->start + len + i - 1) != SUCCESS)
			return (2);
	}
	return (handle_token(tokenize, len));
}

int handle_token(t_For_tokenize *tokenize, int len)
{
	while (ft_isspace(*tokenize->start))
		tokenize->start++;
	tokenize->tokens[tokenize->token_count] = ft_strndup(tokenize->start, len);
	if (!tokenize->tokens[tokenize->token_count])
	{
		all_free(&tokenize->tokens);
		log_errors("Failed to ft_strndup in tokenize_input", "");
		return (FAIL);
	}
	tokenize->start += len;

	tokenize->tokens = ft_realloc_double(tokenize->tokens, tokenize->token_count, &tokenize->buffsize);
	if (!tokenize->tokens)
		return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
	if (ft_strcmp(tokenize->tokens[tokenize->token_count], "<<") == 0)
	{
		tokenize->token_count++;
		while (ft_isspace(*tokenize->start))
			tokenize->start++;
		tokenize->heredoc_quoted[tokenize->heredoc_count] =
			(*tokenize->start == '\'' || *tokenize->start == '"');
		if (handle_special_tokens(tokenize) == FAIL)
			return (FAIL);
		if (tokenize->heredoc_quoted[tokenize->heredoc_count])
		{
			char *limiter_without_quotes = rm_quotes(tokenize->tokens[tokenize->token_count - 1]);
			if (!limiter_without_quotes)
				return (log_errors("Failed to remove quotes from heredoc limiter", ""));
			free(tokenize->tokens[tokenize->token_count - 1]);
			tokenize->tokens[tokenize->token_count - 1] = limiter_without_quotes;
		}
		tokenize->heredoc_limiters[tokenize->heredoc_count] = tokenize->tokens[tokenize->token_count - 1];
		if (readline_again_for_heredoc(tokenize) != SUCCESS)
			return (FAIL);
		tokenize->heredoc_count++;
		tokenize->heredoc_limiters = ft_realloc_double(tokenize->heredoc_limiters, tokenize->heredoc_count, &tokenize->heredoc_buffsize);
	}
	else
		tokenize->token_count++;
	return (SUCCESS);
}
