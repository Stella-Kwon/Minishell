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

char **initialize_tokenization(t_For_tokenize *tokenize)
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

int process_token_and_check_error(t_For_tokenize *tokenize,
								  int *last_exit_code, char ***local_env, char **tmp_input)
{
	int error_code;

	(void)local_env;
	error_code = handle_special_tokens(tokenize);
	if (error_code != SUCCESS)
	{
		if (error_code == 2)
		{
			// ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
			*last_exit_code = 2;
		}
		else if (error_code == 3)
			*last_exit_code = 1;
		all_free(&tokenize->tokens);
		*tmp_input = tokenize->input;
		return (FAIL);
	}
	return (SUCCESS);
}

char **tokenize_input(char **input, int *last_exit_code,
					  char ***local_env, char **tmp_input)
{
	t_For_tokenize tokenize;

	tokenize.input = *input;
	tokenize.start = *input;
	tokenize.buffsize = BUFFSIZE;
	tokenize.error_code = 0;
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
		if (process_token_and_check_error(&tokenize, last_exit_code,
										  local_env, tmp_input) == FAIL)
			return (NULL);
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	*tmp_input = tokenize.input;
	return (tokenize.tokens);
}
