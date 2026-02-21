/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 21:54:21 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char **initialize_tokens(char ***tokens, int size)
{
	*tokens = NULL;
	*tokens = ft_calloc(size, sizeof(char *));
	if (!*tokens)
	{
		log_errors("Failed to allocate memory for tokenize->tokens", "");
		return (NULL);
	}
	return (*tokens);
}

static int process_token_and_check_error(t_For_tokenize *tokenize, int *last_exit_code)
{

	int error_code = handle_special_tokens(tokenize);
	if (error_code != SUCCESS)
	{
		if (error_code == 2)
			*last_exit_code = 2;
		else if (error_code == 3)
			*last_exit_code = 1;
		all_free(&tokenize->tokens);
		return (FAIL);
	}
	return (SUCCESS);
}

static int check_first_input(t_For_tokenize *tokenize)
{
	while (ft_isspace(*(tokenize->start)))
		tokenize->start++;
	if (*(tokenize->start) && *(tokenize->start) == '|')
	{
		if (*(tokenize->start + 1) && *(tokenize->start + 1) == '|')
			handle_258_exitcode_print("`||'");
		else
			handle_258_exitcode_print("`|'");
	}
	else if (*(tokenize->start) && *(tokenize->start) == '&')
	{
		if (*(tokenize->start + 1) && *(tokenize->start + 1) == '&')
			handle_258_exitcode_print("`&&'");
		else
			handle_258_exitcode_print("`&'");
	}
	else
		return (SUCCESS);
	return (FAIL);
}

static void put_data_in_result(t_For_tokenize tokenize, t_TokenizeResult *result)
{
	result->tokens = tokenize.tokens;
	result->heredoc_bodies = tokenize.heredoc_bodies;
	result->heredoc_limiters = tokenize.heredoc_limiters;
	result->heredoc_quoted = tokenize.heredoc_quoted;
	result->heredoc_count = tokenize.heredoc_count;
	result->current_heredoc_index = 0;
}

static int initialize_tokenize(char **tmp_input, t_For_tokenize *tokenize,
							   char ***env, int *last_exit_code)
{
	tokenize->input = *tmp_input;
	tokenize->start = *tmp_input;
	tokenize->buffsize = TOKEN_BUFFSIZE;
	tokenize->tokens = initialize_tokens(&tokenize->tokens, tokenize->buffsize);
	if (!tokenize->tokens)
	{
		log_errors("ft_calloc failed during initializing_tokenzie", "");
		return (FAIL);
	}
	tokenize->token_count = 0;
	tokenize->heredoc_bodies = ft_calloc(HEREDOC_BUFFSIZE, sizeof(char *));
	tokenize->heredoc_limiters = ft_calloc(HEREDOC_BUFFSIZE, sizeof(char *));
	tokenize->heredoc_quoted = ft_calloc(HEREDOC_BUFFSIZE, sizeof(int));
	if (!tokenize->heredoc_bodies || !tokenize->heredoc_limiters
		|| !tokenize->heredoc_quoted)
	{
		log_errors("ft_calloc failed during initializing_tokenzie", "");
		return (FAIL);
	}
	tokenize->heredoc_buffsize = HEREDOC_BUFFSIZE;
	tokenize->heredoc_count = 0;
	tokenize->env = env;
	tokenize->last_exit_code = last_exit_code;
	return (SUCCESS);
}

static void initialize_tokenize_result(t_For_tokenize *tokenize, t_TokenizeResult *result)
{
	result->tokens = tokenize->tokens;
	result->heredoc_bodies = NULL;
	result->heredoc_limiters = NULL;
	result->heredoc_count = 0;
	result->current_heredoc_index = 0;
}

char **tokenize_input(char **tmp_input, t_TokenizeResult *result, char ***env, int *last_exit_code)
{
	t_For_tokenize tokenize;

	if (initialize_tokenize(tmp_input, &tokenize, env, last_exit_code) == FAIL)
	{
		result->tokens = NULL;
		result->heredoc_limiters = NULL;
		result->heredoc_bodies = NULL;
		result->heredoc_count = 0;
		result->current_heredoc_index = 0;
		return (NULL);
	}
	initialize_tokenize_result(&tokenize, result);
	if (check_first_input(&tokenize) != SUCCESS)
	{
		*last_exit_code = 2;
		all_free(&tokenize.tokens);
		result->tokens = NULL;
		return (NULL);
	}
	while (*tokenize.start)
	{
		if (process_token_and_check_error(&tokenize, last_exit_code) == FAIL)
		{
			result->tokens = NULL;
			return (NULL);
		}
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	put_data_in_result(tokenize, result);
	return (tokenize.tokens);
}
