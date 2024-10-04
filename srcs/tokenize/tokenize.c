/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:01:54 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	store_str(char **tokens, char **start, int *token_count, int *buffsize)
{
	if (!ft_isspace(**start) && **start != '\0')
	{
		tokens[*token_count] = store_words(start);
		if (!tokens[*token_count])
		{
			all_free(&tokens);
			return (log_errors("Failed to store word", ""));
		}
		(*token_count)++;
		tokens = ft_realloc(tokens, *token_count, buffsize);
		if (!tokens)
			return (log_errors("Failed to \"reallocate\" memory for tokens", \
					""));
	}
	return (SUCCESS);
}

int	handle_special_tokens(int *buffsize, char **tokens, char **start, \
	int *token_count, char **input)
{
	if (**start == '\'')
		return (handle_set(tokens, start, token_count, input, '\''));
	else if (**start == '"')
		return (handle_set(tokens, start, token_count, input, '"'));
	else if (**start == '(' || **start == ')')
		return handle_set(tokens, start, token_count, input, ')');
	else if (**start == '|')
		return (handle_pipe_and_or(input, tokens, start, token_count));
	else if (**start == '&')
		return (handle_And_and_background(input, tokens, start, token_count));
	else if (**start == '<')
		return (handle_input_redirection(tokens, start, token_count));
	else if (**start == '>')
		return (handle_output_redirection(tokens, start, token_count));
	else if (**start == '$')
		return (handle_token(tokens, start, token_count, 1));
	else if (ft_isspace(**start))
	{
		while (ft_isspace(**start))
			(*start)++;
		return (SUCCESS);
	}
	else
	{
		if (store_str(tokens, start, token_count, buffsize) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

char	**initialize_tokenization(int buffsize, int *token_count)
{
	char	**tokens;

	tokens = NULL;
	tokens = malloc(buffsize * sizeof(char *));
	if (!tokens)
	{
		log_errors("Failed to allocate memory for tokens", "");
		return (NULL);
	}
	*token_count = 0;
	return (tokens);
}

char	**tokenize_input(char **input)
{
	char	*start;
	int		token_count;
	int		buffsize;
	char	**tokens;

	start = *input;
	buffsize = BUFFSIZE;
	tokens = initialize_tokenization(buffsize, &token_count);
	if (!tokens)
		return (NULL);
	if (check_first_input(&start) != SUCCESS)
		return (NULL);
	while (*start)
	{
		if (handle_special_tokens(&buffsize, tokens, &start, \
			&token_count, input) != SUCCESS)
		{
			all_free(&tokens);
			return (NULL);
		}
	}
	tokens[token_count] = NULL;
	return (tokens);
}
