/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:09:30 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/20 16:12:30 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	error_handling_in_middle_operator(t_For_tokenize *tokenize, \
int len, int *i)
{
	if ((*(tokenize->start + len + *i) == '<' && \
		*(tokenize->start + len + *i + 1) == '<' && \
		*(tokenize->start + len + *i + 1) == '<'))
		*i += 3;
	else if ((*(tokenize->start + len + *i) == '<' && \
			*(tokenize->start + len + *i + 1) == '<'))
		*i += 2;
	else if ((*(tokenize->start + len + *i) == '>' && \
			*(tokenize->start + len + *i + 1) == '>'))
		*i += 2;
	else if ((*(tokenize->start + len + *i) == '<') || \
			(*(tokenize->start + len + *i) == '>'))
		(*i)++;
}

int	handle_pipe_and_or(t_For_tokenize *tokenize)
{
	int	len;
	int	i;

	i = 0;
	if (*(tokenize->start + 1) == '|')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(tokenize->start + len + i)))
		i++;
	if (*(tokenize->start + len + i) == '\0')
	{
		i = check_operation_next(tokenize);
		if (i != SUCCESS)
			return (i);
	}
	else
	{
		error_handling_in_middle_operator(tokenize, len, &i);
		if (redirect_operation_error(tokenize->start + len + i) != SUCCESS)
			return (2);
	}
	return (handle_token(tokenize, len));
}

int	handle_and_and_background(t_For_tokenize *tokenize)
{
	int	len;
	int	i;

	i = 0;
	if (*(tokenize->start + 1) == '&')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(tokenize->start + len + i)))
		i++;
	if (*(tokenize->start + len + i) == '\0')
	{
		i = check_operation_next(tokenize);
		if (i != SUCCESS)
			return (i);
	}
	else
	{
		error_handling_in_middle_operator(tokenize, len, &i);
		if (redirect_operation_error(tokenize->start + len + i) != SUCCESS)
			return (2);
	}
	return (handle_token(tokenize, len));
}

int	handle_token(t_For_tokenize *tokenize, int len)
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
	tokenize->token_count++;
	tokenize->tokens = ft_realloc_double(tokenize->tokens, \
	tokenize->token_count, &tokenize->buffsize);
	if (!tokenize->tokens)
		return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
	tokenize->start += len;
	return (SUCCESS);
}

int	handle_set(t_For_tokenize *tokenize, char ref)
{
	if (*tokenize->start == ')')
		return (handle_258_exitcode_print(")"));
	tokenize->tokens[tokenize->token_count] = check_set(tokenize, ref);
	if (!tokenize->tokens[tokenize->token_count])
	{
		all_free(&tokenize->tokens);
		log_errors("Failed to handle set token in tokenize_input", "");
		return (FAIL);
	}
	tokenize->token_count++;
	tokenize->tokens = ft_realloc_double(tokenize->tokens, \
	tokenize->token_count, &tokenize->buffsize);
	if (!tokenize->tokens)
		return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
	if (*tokenize->start == '(')
	{
		while (ft_isspace(*tokenize->start))
			tokenize->start++;
		if (!*tokenize->start)
			return (handle_258_exitcode_print(")"));
	}
	return (SUCCESS);
}
