/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 15:40:25 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"




t_ASTNode	*execute(int *last_exit_code, t_ASTNode **root)
{
	t_ASTNode	*tmp_root;
	t_ASTNode	*set_root;
	t_ASTNode	*get_root;
	int			exit;

	tmp_root = *root;
	set_root = *root;
	get_root = *root;
	// printf("nodetype : %d\n", tmp_root->type);
	// printf("tmpnode : %s\n", tmp_root->command->cmd);
	set_last_exitcode_and_root(&set_root, *last_exit_code, &set_root);
	exit = ast_node_execution(root);
	if (exit == -1 || exit == 130)
	{
		*last_exit_code = 1;
		if (exit == 130)
			*last_exit_code = 130;
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

	(void) last_exit_code;
	tmp_tokens = tokens;
	root = parse_to_nodes(tokens, env);
	if (tmp_tokens)
	{
		// int i  = 0;
		// while (tmp_tokens[i])
		// {
		// 	printf("token : %s\n", tmp_tokens[i]);
		// 	i++;
		// }
		all_free(&tmp_tokens);
	}
	// if (!tmp_tokens)
	// 	printf("token tmp_null\n");
	if (!root)
		return (NULL);
	printf("\n\n----------print start----------\n\n");
	print_astnode(root, 0); // AST 노드 출력
	printf("\n\n=================================\n\n");
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
		// if (tokens)
		// 	printf("tokens is not null : %s \n", *tokens);
	}
	// if (tokens)
	// 	all_free(&tokens);
	// all_free(&local_env);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	int	last_exit_code;

	(void)argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	if (exec_in_loop(env, &last_exit_code) != SUCCESS)
		return (FAIL);
	rl_clear_history();
	return (0);
}