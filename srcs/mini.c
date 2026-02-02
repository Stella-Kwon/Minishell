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

volatile sig_atomic_t	g_interrupt_signal = FALSE;
volatile sig_atomic_t	g_no_child = TRUE;
volatile sig_atomic_t	g_child_count = 0;

t_ASTNode	*execute(int *last_exit_code, t_ASTNode **root)
{
	int			exit;

	set_last_exitcode_and_root(root, *last_exit_code, root);
	exit = ast_node_execution(root);
	*last_exit_code = exit;
	get_last_exitcode(root, last_exit_code);
	if (*root)
		free_astnode(root);
	return (*root);
}

t_ASTNode	*parse_and_execute(char **tokens, char ***env, int *last_exit_code)
{
	t_ASTNode	*root;
	int			ret;

	root = parse_to_nodes(tokens, env);
	if (tokens)
		all_free(&tokens);
	if (!root)
		return (NULL);
	ret = heredoc_preprocess(&root, *env, *last_exit_code);
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

int	exec_in_loop(char **env, int *last_exit_code)
{
	char	**local_env;
	char	*input;
	char	**tokens;

	if (local_env_copy(env, &local_env) == FAIL)
		return (FAIL);
	while (1)
	{
		signal_setup();
		input = get_user_input(last_exit_code, &local_env);
		if (!input)
			continue ;
		tokens = process_input_to_tokens(input, last_exit_code, \
										&local_env);
		if (!tokens)
			continue ;
		parse_and_execute(tokens, &local_env, last_exit_code);
	}
	all_free(&local_env);
	return (SUCCESS);
}

int	main(int argc, char **argv, char **env)
{
	int	last_exit_code;

	(void)argv;
	last_exit_code = 0;
	if (argc != 1)
		return (1);
	setup_terminal();
	signal_setup();
	if (exec_in_loop(env, &last_exit_code) != SUCCESS)
		return (FAIL);
	rl_clear_history();
	return (0);
}
