/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:47:27 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ASTNode *execute(int *last_exit_code, t_ASTNode **root)
{
	int exit;

	set_last_exitcode_and_root(root, *last_exit_code, root);
	exit = ast_node_execution(root);
	*last_exit_code = exit;
	get_last_exitcode(root, last_exit_code);
	if (*root)
		free_astnode(root);
	return (*root);
}

t_ASTNode *parse_and_execute(t_TokenizeResult *result, char ***env, int *last_exit_code)
{
	t_ASTNode *root;
	int ret;

	root = parse_to_nodes(result, env);
	if (result->tokens)
		all_free(&(result->tokens));
	if (!root)
		return (NULL);
	ret = prepare_heredoc_herestring(&root, *env, *last_exit_code);
	if (ret != SUCCESS)
	{
		if (ret == 1)
			*last_exit_code = 1;
		free_astnode(&root);
		return (NULL);
	}
	if (execute(last_exit_code, &root) == NULL)
		return (NULL);
	return (root);
}

int exec_in_loop(char **env, int *last_exit_code)
{
	char **local_env;
	const char *input;
	t_TokenizeResult result;

	if (local_env_copy(env, &local_env) == FAIL)
		return (FAIL);
	while (1)
	{
		signal_setup();
		input = get_user_input();
		if (input == (const char *)-1) // ctrl +c
			continue;
		if (!input) //ctrl+d
		{
			all_free(&local_env);
			return (SUCCESS);
		}
		if (input[0] == '\0')
			continue;
		result = process_input_to_tokens(input, &local_env, last_exit_code);
		if (!result.tokens)
			continue;
		parse_and_execute(&result, &local_env, last_exit_code);
	}
	all_free(&local_env);
	return (SUCCESS);
}

int main(int argc, char **argv, char **env)
{
	int last_exit_code;

	(void)argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	setup_terminal();
	signal_setup();
	if (exec_in_loop(env, &last_exit_code) != SUCCESS)
		return (FAIL);
	// rl_clear_history();
	return (0);
}
