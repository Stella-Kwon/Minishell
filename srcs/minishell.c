/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:10:56 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 05:58:30 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char		*input;
	char		**tokens;
	t_ASTNode	*root;

	(void)argv;
	(void)env;
	(void)root;
	if (argc != 1)
		return (1);
	set_ehcoctl(1);
	while (1)
	{
		int_signal();
		input = readline("MINISHELL > ");
		if (!input)
		{
			ft_putstr_fd("\033[A", 1);
			ft_putstr_fd("\033[K", 1);
			ft_putstr_fd("\033[1G", 1);
			ft_putstr_fd("MINISHELL > exit\n", 2);
			exit(0);
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		tokens = tokenize_input(&input);
		if (!tokens)
		{
			free(input);
			continue ;
		}

		for (int i = 0; tokens[i]; i++)
			printf("tokens[%d] : %s\n", i, tokens[i]);
		root = parse_to_Nodes(tokens, env);
		if (!root)
		{
			free(input);
			free(tokens);
			continue ;
		}
		// execute_node(&root);
		free(input);
		free(tokens);
	}
	return (0);
}
