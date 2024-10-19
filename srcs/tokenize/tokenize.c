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

int	handle_special_tokens(t_For_tokenize *tokenize)
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
		if (store_str(tokenize) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

char	**initialize_tokenization(t_For_tokenize *tokenize)
{
	tokenize->tokens = NULL;
	tokenize->tokens = ft_calloc(tokenize->buffsize, sizeof(char *));
	if (!tokenize->tokens)
	{
		log_errors("Failed to allocate memory for tokens", "");
		return (NULL);
	}
	tokenize->token_count = 0;
	return (tokenize->tokens);
}

int signal_error(t_For_tokenize *tokenize, int *last_exit_code, \
char ***local_env)
{
	if (g_received_signal == 130)
	{
		*last_exit_code = 130;
		g_received_signal = 0;
		return (FAIL);
	}
	ft_putstr_fd("\nminishell: \
	syntax error: unexpected end of file\nexit\n", 2);
	all_free(local_env);
	free_one((void **)&tokenize->tmp_input);
	all_free(&tokenize->tokens);
	exit(2);
}

int exit_check(t_For_tokenize *tokenize, \
int *last_exit_code, char ***local_env)
{
	int	exitcode;

	exitcode = handle_special_tokens(tokenize);
	if (exitcode != SUCCESS)
	{
		if (exitcode == 2)
			*last_exit_code = 2;
		else if (exitcode == 3)
		{
			if (signal_error(tokenize, last_exit_code, \
			local_env) != SUCCESS)
				return (FAIL);
		}
		else if (exitcode == 130)
			*last_exit_code = 1;
		all_free(&tokenize->tokens);
		return (FAIL);
	}
	return (SUCCESS);
}

char	**tokenize_input(char **input, int *last_exit_code, \
char ***local_env)
{
	t_For_tokenize	tokenize;

	tokenize.tmp_input = *input;
	tokenize.input = *input;
	tokenize.start = *input;
	tokenize.buffsize = BUFFER_SIZE;
	tokenize.tokens = initialize_tokenization(&tokenize);
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
		if (exit_check(&tokenize, last_exit_code, \
		local_env) == FAIL)
			return (NULL);
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	return (tokenize.tokens);
}
