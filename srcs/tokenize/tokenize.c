/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/20 16:16:39 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_interrupt_signal;

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

int	signal_error(t_For_tokenize *tokenize, int *last_exit_code, \
char ***local_env, char **tmp_input)
{
	if (g_interrupt_signal == TRUE)
	{
		all_free(local_env);
		all_free(&tokenize->tokens);
		*tmp_input = tokenize->input;
		*last_exit_code = 130;
		g_interrupt_signal = FALSE;
		return (FAIL);
	}
	ft_putstr_fd("minishell: syntax error: unexpected end of file\nexit\n", 2);
	all_free(local_env);
	*tmp_input = tokenize->input;
	all_free(&tokenize->tokens);
	exit(2);
}

int	exit_check(t_For_tokenize *tokenize, \
int *last_exit_code, char ***local_env, char **tmp_input)
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
			local_env, tmp_input) != SUCCESS)
				return (FAIL);
		}
		all_free(&tokenize->tokens);
		*tmp_input = tokenize->input;
		return (FAIL);
	}
	return (SUCCESS);
}

char **tokenize_input(char **input, int *last_exit_code, \
char ***local_env, char **tmp_input)
{
	t_For_tokenize	tokenize;

	tokenize.input = *input;
	tokenize.start = *input;
	tokenize.buffsize = BUFFSIZE;
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
		local_env, tmp_input) == FAIL)
			return (NULL);
	}
	tokenize.tokens[tokenize.token_count] = NULL;
	*tmp_input = tokenize.input;
	return (tokenize.tokens);
}
