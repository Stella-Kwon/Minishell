/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 23:11:39 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 22:07:05 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int local_env_copy(char **env, char ***local_env)
{
	int env_len;

	env_len = get_str_len(env);
	*local_env = ft_calloc(env_len + 1, sizeof(char *));
	if (!*local_env)
	{
		log_errors("Failed calloc in main", "");
		return (FAIL);
	}
	if (copy_envp(*local_env, env, env_len) == FAIL)
	{
		all_free(local_env);
		return (FAIL);
	}
	return (SUCCESS);
}

const char *get_user_input(void)
{
	const char *input;

	input = read_line_safe("minishell > ");
	if (!input || input == (char *)-1)
	{
		if (input == (char *)-1)
			return ((const char *)-1);
		return (NULL);
	}
	if (isatty(STDIN_FILENO))
		add_history(input);
	return (input);
}

t_TokenizeResult process_input_to_tokens(const char *input, char ***env,
										 int *last_exit_code)
{
	static t_TokenizeResult result;
	char **tokens;
	char *tmp_input;

	tmp_input = (char *)input;
	tokens = tokenize_input(&tmp_input, &result, env, last_exit_code);
	if (!tokens)
		return (result);
	return (result);
}
