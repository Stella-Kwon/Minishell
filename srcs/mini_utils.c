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

int	local_env_copy(char **env, char ***local_env)
{
	int	env_len;

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

char	*get_user_input(int *last_exit_code, char ***local_env)
{
	char	*input;

	input = readline("minishell > ");
	if (!input)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		rl_clear_history();
		all_free(local_env);
		exit(0);
	}
	if (input[0] == '\0')
		return (NULL);
	add_history(input);
	if (check_input(input) == FAIL)
	{
		*last_exit_code = 2;
		return (NULL);
	}
	return (input);
}

char	**process_input_to_tokens(char *input, int *last_exit_code, \
									char ***local_env_copy)
{
	char	**tokens;
	char	*tmp_input;

	tmp_input = input;
	tokens = tokenize_input(&input, last_exit_code, \
							local_env_copy, &tmp_input);
	if (tmp_input)
		free_one((void **)&tmp_input);
	if (!tokens)
		return (NULL);
	return (tokens);
}
