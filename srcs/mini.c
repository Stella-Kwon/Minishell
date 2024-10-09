/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 23:16:35 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **env) //(int argc, char **argv, char **env)
{
	char *input;
	char **tokens;
	t_ASTNode *root;
	int last_exit_code;

	(void)argv;
	root = NULL;
	last_exit_code = 0;
	if (argc != 1)
		return (1);

	set_ehcoctl(1);
	while (1) // 무한 루프
	{
		init_signal();
		input = readline("MINISHELL > "); // 입력 받기
		if (!input) // ctrl + d를 눌렀을 경우
		{
			ft_putstr_fd("\033[A",1);					  // 한 줄 위로 이동
			ft_putstr_fd("\033[K",1);					  // 현재 줄 지우기
			ft_putstr_fd("\033[1G",1);					 // 커서를 줄의 시작으로 이동 (1번째 열)
			ft_putstr_fd("MINISHELL > exit\n", 2); // exit 메시지 출력
			rl_clear_history();
			exit(0); // 바로 종료
		}
		if (input[0] == '\0') // 입력이 없는 경우 엔터쳤을떄
		{
			free(input);
			input = NULL;
			continue ;
		}
		add_history(input);
		if (check_input(input) == FAIL) //258exitcode?
		{
			free(input);
			input = NULL;
			continue;
		}
		char *tmp_input = input;
		tokens = tokenize_input(&input);
		if (!tokens)
		{
			if(tmp_input){
				free(tmp_input);
				tmp_input  = NULL;
			}
			continue ;
		}

		// 토큰 출력
		for (int i = 0; tokens[i]; i++)
			printf("tokens[%d] : %s\n", i, tokens[i]);
		
		char **tmp_tokens = tokens;
		root = parse_to_nodes(tokens, env); // 파싱
		if (!root)							// 파싱 실패한 경우
		{
			if (tmp_input){
				free(tmp_input);
				tmp_input = NULL;
			}
			if (tmp_tokens)
				all_free(&tmp_tokens);
			continue;
		}
		printf("\n\n----------print start----------\n\n");
		print_astnode(root, 0); // AST 노드 출력
		printf("\n\n=================================\n\n");
		init_execution_signal();
		set_last_exitcode(&root, last_exit_code);
		t_ASTNode *tmp_root = root;
		// printf("root: %p\n", (void *)tmp_root);
		// printf("root: %s\n", tmp_root->command->cmd);
		if (ast_node_execution(&root) != SUCCESS)
		{
			// printf("sdf\n");
			if (tmp_root)
			{
				// printf("root\n");
				free_astnode(&tmp_root);
			}
				
			if (tmp_input)
			{
				// printf("input\n");
				free(tmp_input);
				tmp_input = NULL;
			}
			if (tmp_tokens)
			{
				// printf("token\n");
				// printf("token : %s", *tmp_tokens);
				all_free(&tmp_tokens);
			}
				
			continue;
		}

		// 사용한 메모리 해제
		if (tmp_root)
			free_astnode(&tmp_root);
		if (tmp_input){
			free(tmp_input);
			tmp_input = NULL;
		}
		if (tmp_tokens)
			all_free(&tmp_tokens);
	}
	return (0);
}
