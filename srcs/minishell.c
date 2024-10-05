/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:10:56 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/03 22:46:48 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **env) //(int argc, char **argv, char **env)
{
    char *input;
    char **tokens;
    ASTNode *root;
    
    (void)argv;
    (void)env;
    (void)root;
    if (argc != 1)
        return (1);
    set_ehcoctl(1);
    while (1) // 무한 루프
    {
        init_signal();
        input = readline("MINISHELL > "); // 입력 받기
        if (!input) // ctrl + d를 눌렀을 경우 // 근데 exeve이후에 알아서 input이 없다고 전달돼서... 아마 \n이 같이 전달돼나? 
        {
            ft_putstr_fd("\033[A",1);                      // 한 줄 위로 이동
            ft_putstr_fd("\033[K",1);                      // 현재 줄 지우기
            ft_putstr_fd("\033[1G",1);                     // 커서를 줄의 시작으로 이동 (1번째 열)
            ft_putstr_fd("MINISHELL > exit\n", 2); // exit 메시지 출력
            exit(0);                    // 바로 종료
        }
        if (input[0] == '\0') // 입력이 없는 경우 엔터쳤을떄
        {
            free(input);
            continue;
        }
        add_history(input);
        tokens = tokenize_input(&input);
        if (!tokens)
        {
            free(input);
            continue;
        }
        // 토큰 출력
        for (int i = 0; tokens[i]; i++)
            printf("tokens[%d] : %s\n", i, tokens[i]);

        // printf("tokens : %s\n", *tokens);
        root = parse_to_Nodes(tokens, env); // 파싱
        // printf("\n\nroot : %p\n\n", root);
        if (!root)                          // 파싱 실패한 경우
        {
            free(input);
            free(tokens);
            continue;
        }
        printf("\n\n----------print start----------\n\n");
        print_ASTNode(root, 0); // AST 노드 출력
        printf("\n\n=================================\n\n");
        init_execution_signal();
        if (ast_node_execution(&root) == FAIL)
        {
            free_ASTNODE(&root);
            free(input);
            free(tokens);
            continue;
        }
        // 사용한 메모리 해제
        free_ASTNODE(&root);
        free(input);
        free(tokens); // tokens 배열 메모리 해제 (필요한 경우)
        // free_ASTNode(root); // 필요하다면 AST 노드 메모리 해제
    }
    return (0);
}
