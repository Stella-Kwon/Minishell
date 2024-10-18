/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 01:30:53 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_user_input(int *last_exit_code, char **local_env)
{
	char	*input;

	init_signal();
	input = readline("minishell > ");
	if (!input)
	{
		// ft_putstr_fd("\033[A\033[K\033[1Gminishell > exit\n", 2);
		rl_clear_history();
		free(local_env);
		exit(0);
	}
	if (input[0] == '\0')
	{
		free(input);
		return (NULL);
	}
	add_history(input);
	if (check_input(input) == FAIL)
	{
		*last_exit_code = 2;
		free(input);
		return (NULL);
	}
	return (input);
}

char	**process_input_to_tokens(char *input, int *last_exit_code)
{
	char	**tokens;
	char	*tmp_input;

	tmp_input = input;
	tokens = tokenize_input(&input, last_exit_code);
	if (tmp_input)
	{
		free(tmp_input);
		tmp_input = NULL;
	}
	if (!tokens)
		return (NULL);
	return (tokens);
}

t_ASTNode	*parse_and_execute(char **tokens, char ***local_env, int *last_exit_code)
{
	t_ASTNode	*root;
	t_ASTNode	*tmp_root;
	t_ASTNode	*set_root;
	t_ASTNode	*get_root;
	char		**tmp_tokens;

	tmp_tokens = tokens;
	root = parse_to_nodes(tokens, local_env);
	tmp_root = root;
	set_root = root;
	get_root = root;
	if (tmp_tokens)
		all_free(&tmp_tokens);
	if (!root)
		return (NULL);
	// printf("\n\n----------print start----------\n\n");
	// print_astnode(root, 0); // AST 노드 출력
	// printf("\n\n=================================\n\n");
	set_last_exitcode(&set_root, *last_exit_code);
	if (ast_node_execution(&root) == -1)
	{
		*last_exit_code = 1;
		if (tmp_root)
			free_astnode(&tmp_root);
		return (root);
	}
	get_last_exitcode(&get_root, last_exit_code);
	if (tmp_root)
		free_astnode(&tmp_root);
	return (root);
}

int	main(int argc, char **argv, char **env)
{
	int		last_exit_code;
	int		env_len;
	char	*input;
	char	**tokens;
	char	**local_env;

	(void) argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	set_ehcoctl(1);
	env_len = get_str_len(env);
	local_env = ft_calloc(env_len + 2, sizeof(*local_env));
	if (!local_env)
	{
		log_errors("Failed calloc in main", "");
		return (FAIL);
	}
	if (copy_envp(local_env, env, env_len) == FAIL)
	{
		delete_str_array(&local_env);
		return (FAIL);
	}
	while (1)
	{
		input = get_user_input(&last_exit_code, local_env);
		if (!input)
			continue ;
		tokens = process_input_to_tokens(input, &last_exit_code);
		if (!tokens)
			continue ;
		parse_and_execute(tokens, &local_env, &last_exit_code);
	}
	free(local_env);
	return (0);
}

// // 토큰 출력
// for (int i = 0; tokens[i]; i++)
// 	printf("tokens[%d] : %s\n", i, tokens[i]);

// printf("\n\n----------print start----------\n\n");
// print_astnode(root, 0); // AST 노드 출력
// printf("\n\n=================================\n\n");