/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:20:22 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ASTNode	*execute(int *last_exit_code, t_ASTNode **root)
{
	t_ASTNode	*tmp_root;
	t_ASTNode	*set_root;
	t_ASTNode	*get_root;

	tmp_root = *root;
	set_root = *root;
	get_root = *root;
	set_last_exitcode_and_root(&set_root, *last_exit_code, &set_root);
	if (ast_node_execution(root) == -1)
	{
		*last_exit_code = 1;
		if (tmp_root)
			free_astnode(&tmp_root);
		return (*root);
	}
	get_last_exitcode(&get_root, last_exit_code);
	if (tmp_root)
		free_astnode(&tmp_root);
	return (*root);
}

t_ASTNode	*parse_and_execute(char **tokens, char ***env, int *last_exit_code)
{
	t_ASTNode	*root;
	char		**tmp_tokens;

	tmp_tokens = tokens;
	root = parse_to_nodes(tokens, env);
	if (tmp_tokens)
		all_free(&tmp_tokens);
	if (!root)
		return (NULL);
	if (execute(last_exit_code, &root) == NULL)
		return (NULL);
	return (root);
}

int	exec_in_loop(char **env, int *last_exit_code)
{
	char	**local_env;
	char	*input;
	char	**tokens;

	if (local_env_copy(env, &local_env) == FAIL)
		return (FAIL);
	while (1)
	{
		input = get_user_input(last_exit_code, &local_env);
		if (!input)
		{
			free_one((void **)input);
			continue ;
		}
		tokens = process_input_to_tokens(input, last_exit_code, \
										&local_env);
		if (!tokens)
			continue ;
		parse_and_execute(tokens, &local_env, last_exit_code);
	}
	return (SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	int	last_exit_code;

	(void)argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	set_ehcoctl(1);
	if (exec_in_loop(env, &last_exit_code) != SUCCESS)
		return (FAIL);
	return (0);
}