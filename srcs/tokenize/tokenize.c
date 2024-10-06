/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/06 21:04:33 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		tokenize->tokens = ft_realloc(tokenize->tokens, \
									tokenize->token_count, buffsize);
		if (!tokenize->tokens)
			return (log_errors("Failed to \"reallocate\" memory for tokens", \
					""));
	}
	return (SUCCESS);
}

int	handle_whitespace(t_For_tokenize	*tokenize)
{
	while (ft_isspace(*tokenize->start))
		(tokenize->start)++;
	return (SUCCESS);
}

int	handle_special_tokens(int *buffsize, t_For_tokenize *tokenize)
{
	if (*tokenize->start == '\'')
		return (handle_set(tokenize, '\''));
	else if (*tokenize->start == '"')
		return (handle_set(tokenize, '"'));
	else if (*tokenize->start == '(' || *tokenize->start == ')')
		return (handle_set(tokenize, ')'));
	else if (*tokenize->start == '|')
		return (handle_pipe_and_or(tokenize));
	else if (*tokenize->start == '&')
		return (handle_and_and_background(tokenize));
	else if (*tokenize->start == '<')
		return (handle_input_redirection(tokenize));
	else if (*tokenize->start == '>')
		return (handle_output_redirection(tokenize));
	else if (ft_isspace(*(tokenize->start)))
		return (handle_whitespace(tokenize));
	else
	{
		if (store_str(tokenize, buffsize) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

char	**initialize_tokenization(int buffsize, t_For_tokenize *tokenize)
{
	tokenize->tokens = NULL;
	tokenize->tokens = malloc(buffsize * sizeof(char *));
	if (!tokenize->tokens)
	{
		log_errors("Failed to allocate memory for tokens", "");
		return (NULL);
	}
	tokenize->token_count = 0;
	return (tokenize->tokens);
}

char	**tokenize_input(char **input)
{
	int				buffsize;
	t_For_tokenize	tokenize;

	tokenize.input = *input;
	tokenize.start = *input;
	buffsize = BUFFSIZE;
	tokenize.tokens = initialize_tokenization(buffsize, &tokenize);
	if (!tokenize.tokens)
		return (NULL);
	if (check_first_input(&tokenize) != SUCCESS)
		return (NULL);
	while (*tokenize.start)
	{
		if (handle_special_tokens(&buffsize, &tokenize) != SUCCESS)
		{
			all_free(&tokenize.tokens);
			return (NULL);
		}
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	return (tokenize.tokens);
}
