/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 01:34:07 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	main(int argc, char **argv, char **env)
// {
// 	char		*input;
// 	// char		*tmp_input;
// 	// char		**tmp_tokens;
// 	char		**tokens;
// 	t_ASTNode	*root;
// 	int			last_exit_code;

// 	(void)argv;
// 	// *tmp_input = input;
// 	// **tmp_tokens = tokens;
// 	root = NULL;
// 	last_exit_code = 0;
// 	if (argc != 1)
// 		return (1);
// 	set_ehcoctl(1);
// 	while (1)
// 	{
// 		init_signal();
// 		input = readline("minishell > ");
// 		if (!input)
// 		{
// 			ft_putstr_fd("\033[A", 1);
// 			ft_putstr_fd("\033[K", 1);
// 			ft_putstr_fd("\033[1G", 1);
// 			ft_putstr_fd("minishell > exit\n", 2);
// 			rl_clear_history();
// 			exit (0);
// 		}
// 		if (input[0] == '\0')
// 		{
// 			free(input);
// 			input = NULL;
// 			continue ;
// 		}
// 		add_history(input);
// 		if (check_input(input) == FAIL)//258exitcode? 아님 
// 		{
// 			free(input);
// 			input = NULL;
// 			continue ;
// 		}
// 		char *tmp_input = input;
// 		tokens = tokenize_input(&input);
// 		if (!tokens)
// 		{
// 			if (tmp_input)
// 			{
// 				free(tmp_input);
// 				tmp_input = NULL;
// 			}
// 			continue ;
// 		}
// 		char **tmp_tokens = tokens;
// 		root = parse_to_nodes(tokens, &env); // 파싱
// 		if (!root)							// 파싱 실패한 경우
// 		{
// 			if (tmp_input){
// 				free(tmp_input);
// 				tmp_input = NULL;
// 			}
// 			if (tmp_tokens)
// 				all_free(&tmp_tokens);
// 			continue ;
// 		}
// 		init_execution_signal();
// 		t_ASTNode *tmp_root = root;
// 		t_ASTNode *set_root = root;
// 		set_last_exitcode(&set_root, last_exit_code);
// 		// printf("root: %p\n", (void *)tmp_root);
// 		// printf("root: %s\n", tmp_root->command->cmd);
// 		if (ast_node_execution(&root) != SUCCESS)
// 		{
// 			get_last_exitcode(&root, &last_exit_code);
// 			// printf("sdf\n");
// 			if (tmp_root)
// 			{
// 				// printf("root\n");
// 				free_astnode(&tmp_root);
// 			}		
// 			if (tmp_input)
// 			{
// 				// printf("input\n");
// 				free(tmp_input);
// 				tmp_input = NULL;
// 			}
// 			if (tmp_tokens)
// 			{
// 				// printf("token\n");
// 				// printf("token : %s", *tmp_tokens);
// 				all_free(&tmp_tokens);
// 			}		
// 			continue ;
// 		}
// 		get_last_exitcode(&root, &last_exit_code);
// 		// 사용한 메모리 해제
// 		if (tmp_root)
// 			free_astnode(&tmp_root);
// 		if (tmp_input)
// 		{
// 			free(tmp_input);
// 			tmp_input = NULL;
// 		}
// 		if (tmp_tokens)
// 			all_free(&tmp_tokens);
// 	}
// 	return (0);
// }






int main(int argc, char **argv, char **env)
{
    char *input;
    char **tokens;
    t_ASTNode *root;
    int last_exit_code = 0;

    (void)argv;
    root = NULL;
    if (argc != 1)
        return 1;

    while (1)
    {
        init_signal();
        input = readline("minishell > ");
        if (!input)
            break;

        if (input[0] == '\0') {
            free(input);
            continue;
        }

        add_history(input);
        if (check_input(input) == FAIL) {
            free(input);
            continue;
        }

        tokens = tokenize_input(&input);
        if (!tokens) {
            free(input);
            continue;
        }

        root = parse_to_nodes(tokens, &env);
        if (!root) {
            free(input);
            all_free(&tokens);
            continue;
        }

        init_execution_signal();
        set_last_exitcode(&root, last_exit_code);
        if (ast_node_execution(&root) != SUCCESS) {
            get_last_exitcode(&root, &last_exit_code);
            free_astnode(&root);
            free(input);
            all_free(&tokens);
            continue;
        }

        get_last_exitcode(&root, &last_exit_code);
        free_astnode(&root);
        free(input);
        all_free(&tokens);
    }

    return 0;
}
