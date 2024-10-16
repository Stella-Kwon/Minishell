/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 15:43:31 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_user_input(void)
{
	char	*input;

	init_signal();
	input = readline("minishell > ");
	if (!input)
	{
		ft_putstr_fd("\033[A\033[K\033[1Gminishell > exit\n", 2);
		rl_clear_history();
		exit(0);
	}
	if (input[0] == '\0' || check_input(input) == FAIL)
	{
		free(input);
		return (NULL);
	}
	add_history(input);
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

t_ASTNode	*parse_and_execute(char **tokens, char **env, int *last_exit_code)
{
	t_ASTNode	*root;
	t_ASTNode	*tmp_root;
	t_ASTNode	*set_root;
	t_ASTNode	*get_root;
	char		**tmp_tokens;

	tmp_tokens = tokens;
	root = parse_to_nodes(tokens, &env);
	tmp_root = root;
	set_root = root;
	get_root = root;
	if (tmp_tokens)
		all_free(&tmp_tokens);
	if (!root)
		return (NULL);
	set_last_exitcode(&set_root, *last_exit_code);
	ast_node_execution(&root);
	get_last_exitcode(&get_root, last_exit_code);
	if (tmp_root)
		free_astnode(&tmp_root);
	return (root);
}

int	main(int argc, char **argv, char **env)
{
	int		last_exit_code;
	char	*input;
	char	**tokens;

	(void) argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	set_ehcoctl(1);
	while (1)
	{
		input = get_user_input();
		if (!input)
			continue ;
		tokens = process_input_to_tokens(input, &last_exit_code);
		if (!tokens)
			continue ;
		parse_and_execute(tokens, env, &last_exit_code);
	}
	return (0);
}
