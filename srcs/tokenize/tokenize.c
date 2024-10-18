/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 23:09:32 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	tokenize->tokens = ft_calloc(buffsize, sizeof(char *));
	if (!tokenize->tokens)
	{
		log_errors("Failed to allocate memory for tokens", "");
		return (NULL);
	}
	tokenize->token_count = 0;
	return (tokenize->tokens);
}

int	signal_error(t_For_tokenize *tokenize, int *last_exit_code)
{
	if (g_received_signal == 130)
	{
		*last_exit_code = 130;
		g_received_signal = 0;
		return (FAIL);
	}
	ft_putstr_fd("\nminishell: \
	syntax error: unexpected end of file\nexit\n", 2);
	all_free(&tokenize->tokens);
	exit(2);
}

int	exit_check(int *buffsize, t_For_tokenize *tokenize, \
int *last_exit_code)
{
	int	exitcode;

	exitcode = handle_special_tokens(buffsize, tokenize);
	if (exitcode != SUCCESS)
	{
		if (exitcode == 2)
			*last_exit_code = 2;
		else if (exitcode == 3)
		{
			if (signal_error(tokenize, last_exit_code) != SUCCESS)
				return (FAIL);
		}
		else if (exitcode == 130)
			*last_exit_code = 1;
		all_free(&tokenize->tokens);
		return (FAIL);
	}
	return (SUCCESS);
}

char	**tokenize_input(char **input, int *last_exit_code)
{
	int				buffsize;
	t_For_tokenize	tokenize;

	tokenize.input = *input;
	tokenize.start = *input;
	buffsize = BUFFER_SIZE;
	tokenize.tokens = \
	initialize_tokenization(buffsize, &tokenize);
	if (!tokenize.tokens)
		return (NULL);
	if (check_first_input(&tokenize) != SUCCESS)
	{
		*last_exit_code = 2;
		all_free(&tokenize.tokens);
		return (NULL);
	}
	while (*tokenize.start)
	{
		if (exit_check(&buffsize, &tokenize, last_exit_code) == FAIL)
			return (NULL);
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	return (tokenize.tokens);
}
