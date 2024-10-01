/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 01:53:35 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_redirection(Redirection **redir)
{
    if (!(*redir))
        return;
    if ((*redir)->filename)
        free_one((void **)&(*redir)->filename);
    if ((*redir)->heredoc_limiter)
        free_one((void **)&(*redir)->heredoc_limiter);
    if ((*redir)->herestring_str)
        free_one((void **)&(*redir)->herestring_str);
    if ((*redir)->dollar_vari)
        free_one((void **)&(*redir)->dollar_vari);
    free_one((void **)&(*redir));
}

int is_redirection(char **token)
{
    int i;

    i = 0;
    while (token[i])
    {
        if (ft_strcmp(token[i], "<") == 0 || ft_strcmp(token[i], "<<") == 0 || ft_strcmp(token[i], "<<<") == 0 ||
            ft_strcmp(token[i], ">") == 0 || ft_strcmp(token[i], ">>") == 0)
            return (TRUE);
        i++;
    }
    return (FALSE);
}

Redirection *create_redirection()
{
    Redirection *redir = (Redirection *)malloc(sizeof(Redirection));
    if (!redir)
    {
        log_errors("Failed to malloc redirection in create_redirection", "");
        return (NULL);
    }
    redir->infile = -2;
    redir->outfile = -2;
    redir->filename = NULL;
    redir->direction_type = -1;
    redir->heredoc_limiter = NULL;
    redir->herestring_str = NULL;
    redir->dollar_vari = NULL;
    return (redir);
}

void free_ASTNODE(ASTNode **node)
{
    // printf("hi\n\n");
    if (*node)
    {
        if ((*node)->command)
            free_Command(&(*node)->command);
        if ((*node)->pipeline)
            free_one((void **)&(*node)->pipeline);
        if ((*node)->redir)
            free_redirection(&(*node)->redir);
        if ((*node)->right)
            free_ASTNODE(&(*node)->right);
        if ((*node)->left)
            free_ASTNODE(&(*node)->left);
        free_one((void **)&*node);
    }
}

Pipeline *create_pipeline()
{
    Pipeline *pipeline;

    pipeline = (Pipeline *)malloc(sizeof(Pipeline));
    if (!pipeline)
    {
        log_errors("Failed to malloc pipeline in create_pipeline", "");
        return (NULL);
    }
    return (pipeline);
}

int initialize_ASTNode(ASTNode **node, char ***tokens)
{
    if ((*node))
    {
        (*node)->type = 0; // 적절한 기본 타입으로 설정
        (*node)->command = NULL;
        (*node)->pipeline = NULL;
        (*node)->redir = NULL;
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
    if (is_redirection(*tokens))
    {
        (*node)->redir = create_redirection();
        if (parsing_others(tokens, &(*node)->redir, TRUE) == FAIL)
        {
            free_ASTNODE(node);
            return (FAIL);
        }
    }
    return (SUCCESS);
}

ASTNode *create_ASTNode(NodeType type, char ***tokens, ASTNode *left, ASTNode *right, char **env)
{
    ASTNode *ast;
    
    ast = (ASTNode *)malloc(sizeof(ASTNode));
    if (!ast)
    {
        log_errors("Failed to malloc node in create_ASTNode", "");
        return (NULL);
    }
    if (initialize_ASTNode(&ast, tokens) == FAIL)
        return(NULL);
    ast->type = type;
    printf("ast->type : %u\n", ast->type);
    if (!tokens || !*tokens || !**tokens)
    {
        printf("\nNODE_OPERATION\n");
        ast->command = NULL;
    }
    else
    {
        printf("tokens : %s\n", **tokens);
        ast->command = create_command(tokens, env);
        if (!ast->command)
            return (NULL);
    }
    printf("\n AFTER CREATE COMMAND tokens : %s\n\n", **tokens);
    printf("hello\n\n\n");
    ast->pipeline = create_pipeline();
    if (!ast->pipeline)
        return (NULL);
    if (ast->redir == NULL)
    {
        ast->redir = create_redirection();
        if (!ast->redir)
            return (NULL);
    }
    // printf("now\n\n");
    if (ast->command)
    {
        if (parsing_others(&ast->command->args, &ast->redir, FALSE) == FAIL)
        {
            free_ASTNODE(&ast);
            return (NULL);
        }
    }
    printf("now\n\n");
    ast->left = left;
    ast->right = right;
    printf("now\n\n");
    return (ast);
}

void operation_parsing(char ***tokens, ASTNode **left_node, char **env)
{
    printf("-----------------OPERATING PARSING TOKEN : %s\n------------------------", **tokens);
    if (ft_strcmp(**tokens, "&&") == 0)
    {
        (*tokens)++;
        ASTNode *right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
        *left_node = create_ASTNode(NODE_AND, NULL, *left_node, right_node, env);
    }
    else if (ft_strcmp(**tokens, "||") == 0)
    {
        (*tokens)++;
        ASTNode *right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
        *left_node = create_ASTNode(NODE_OR, NULL, *left_node, right_node, env);

    }
    else if (ft_strcmp(**tokens, "|") == 0)
    {
        (*tokens)++;
        ASTNode *right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
        *left_node = create_ASTNode(NODE_PIPE, NULL, *left_node, right_node, env);

    }
    // 괄호 처리
    // else if (ft_strcmp(***tokens, "(") == 0) // 앤 다시 토큰을 쪼개고 해줘야돼
    // {
    //     (*tokens)++;                                  // 다음 토큰으로 이동
    //      토큰나이저 해주고 그 토큰으로
    //     ASTNode *subshell_node = NULL;                // 서브쉘 노드 초기화 연결될꺼니까
    //     subshell_node = parse_to_Nodes(*tokens, env); // 서브쉘의 명령어를 파싱

    //     (*tokens)++; // ')' 다음 토큰으로 이동

    //     // 서브쉘 노드를 현재 노드에 연결
    //     *left_node = create_ASTNode(NODE_SUBSHELL, NULL, *left_node, subshell_node, env);
    // }
    else
    {
        (*tokens)++; // *tokens++에서 괄호 추가
    }
}


ASTNode *parse_to_Nodes(char **tokens, char **env)
{
    ASTNode *left_node;
    if (!tokens || !*tokens)
        return (NULL);
    printf("--------------------start parsing tokens : %s---------------------\n", *tokens);
    if (*tokens)
    {
        left_node = create_ASTNode(NODE_COMMAND, &tokens, NULL, NULL, env);
        if (!left_node)
            return (NULL);
        printf("--------------------passed creating the first left node tokens : %s----------------------\n", *tokens);
    }
    while (*tokens)//left + right 연산자 노드 만들고 나면 나와서 다시 토큰위치다음부터 시작.
    {
        operation_parsing(&tokens, &left_node, env);
    }
    return (left_node); //결국 맨마지막 연산자 노드가 탑이된다. ls | pwd && cd 하면 &&가 맨 마지막이지만 트리구조에선 루트로 이걸타고 실행되는것이 맞아.
}

void print_ASTNode(ASTNode *node, int depth)
{
    // int i;

    // i = 0;
    if (!node)
        return;
    // 현재 노드 정보 출력
    // while (node && i < depth)
    // {
    //     i++;
        printf("\n"); // 들여쓰기
    // }
    // printf("Node Type: %d, Command: %s\n", node->type, node->command ? node->command->name : "NULL");
    printf("Node Type: %d\n", node->type);

    // 명령어가 NULL인지 확인
    if (node->command == NULL)
    {
        printf("Command: NULL\n");
    }
    else
    {
        // 명령어가 NULL이 아닐 경우, 명령어의 이름을 출력
        printf("Command: %s\n", node->command->cmd);
    }
    if (node->redir)
    {
        printf("Redirection details:\n");
        printf("infile: %d\n", node->redir->infile);
        printf("outfile: %d\n", node->redir->outfile);
        printf("filename: %s\n", node->redir->filename ? node->redir->filename : "(NULL)");
        printf("direction_type: %d\n", node->redir->direction_type);
        printf("heredoc_limiter: %s\n", node->redir->heredoc_limiter ? node->redir->heredoc_limiter : "(NULL)");
        printf("herestring_str: %s\n", node->redir->herestring_str ? node->redir->herestring_str : "(NULL)");
        printf("dollar_vari: %s\n", node->redir->dollar_vari ? node->redir->dollar_vari : "(NULL)");
    }
        // 왼쪽 자식 노드 출력
        print_ASTNode(node->left, depth + 1);
    // 오른쪽 자식 노드 출력
    print_ASTNode(node->right, depth + 1);
}
