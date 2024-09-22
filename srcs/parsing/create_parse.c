/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:31:27 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

Pipeline *create_pipeline()
{
    Pipeline *pipeline;

    pipeline = (Pipeline *)malloc(sizeof(Pipeline));
    if (!pipeline)
    {
        log_errors("Failed to malloc pipeline in create_pipeline");
        return (NULL);    
    }
    return (pipeline);
}

ASTNode *create_ASTNode(NodeType type, char **tokens, ASTNode *left, ASTNode *right, char **env)
{
    ASTNode *ast;

    ast = (ASTNode *)malloc(sizeof(ASTNode));
    if (!ast)
    {
        log_errors("Failed to malloc node in create_ASTNode");
        return (NULL);
    }
    ast->type = type;
    ast->command = create_command(tokens, env);
    ast->pipeline = create_pipeline();
    ast->left = left;
    ast->right = right;
    return (ast);
}

// char **split_tokens(char **initial_start_tokens, char *token, int count) // this one has to be free
// {
//     char **res;
//     int i;

//     i = 0;
//     res = (char **)malloc(sizeof(char *) * (count + 1));
//     if (!res)
//     {
//         log_errors("Failed to malloc res in split_tokens function");
//         return (NULL);
//     }
//     while (*initial_start_tokens)
//     {
//         if (*initial_start_tokens == token)
//             break;
//         else
//         {
//             res[i] = ft_strdup(*initial_start_tokens);
//             i++;
//         }
//         *initial_start_tokens++;
//     }
//     res[i] = '\0';
//     return (res);
// }

void operation_parsing(char ***tokens, ASTNode **left_node, char **env)
{
    if (ft_strcmp(**tokens, "&&") == 0)
    {
        *tokens++;
        ASTNode *right_node = parse_to_Nodes(*tokens, env); // 재귀 호출
        *left_node = create_ASTNode(NODE_AND, NULL, *left_node, right_node, env);
    }
    else if (ft_strcmp(**tokens, "||") == 0)
    {
        *tokens++;
        ASTNode *right_node = parse_to_Nodes(*tokens, env); // 재귀 호출
        *left_node = create_ASTNode(NODE_OR, NULL, *left_node, right_node, env);
    }
    else if (ft_strcmp(**tokens, "|") == 0)
    {
        *tokens++;
        ASTNode *right_node = parse_to_Nodes(*tokens, env); // 재귀 호출
        *left_node = create_ASTNode(NODE_PIPE, NULL, *left_node, right_node, env);
    }
    else if (ft_strcmp(***tokens, "(") == 0)
    {
        *tokens++;
        *tokens = tokenize_input(**tokens);
        ASTNode *right_node = parse_to_Nodes(*tokens, env); // 재귀 호출
        *left_node = create_ASTNode(NODE_SUBSHELL, NULL, *left_node, right_node, env);
    }
    else
        *tokens++;
}

ASTNode *parse_to_Nodes(char **tokens, char **env)
{
    ASTNode *left_node;

    if (*tokens && **tokens != '(')
        left_node = create_ASTNode(NODE_COMMAND, *tokens, NULL, NULL, env);
    while (*tokens)
        operation_parsing(&tokens, &left_node, env);
    return (left_node);
}
