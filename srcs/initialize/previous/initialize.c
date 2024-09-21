/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:43:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/05 17:27:52 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//free해주는거 절대적으로 잊지마~~~~ 따로 만들쟈 한번에 다 지워주느거!

Command *create_command()
{
    Command *node;

    node = (Command *)malloc(sizeof(Command));
    if (!node)
    {
        log_errors("failed to allocate Command node");
        return (NULL);
    }
    node->cmd = NULL; // 나중에 빌트인 함수인지 확인하고 실행
    node->args = NULL;
    node->exitcode = 0;
    return (node);
}

ASTNode *create_ASTnode(NodeType type, Command *command, ASTNode *left, ASTNode *right)
{
    ASTNode *node;
    node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node)
    {
        log_errors("failed to allocate ASTNode node");
        return NULL;
    }

    // 필수 필드 초기화
    node->type = type;
    node->command = command;
    node->left = left;
    node->right = right;

    // 기타 필드는 NULL로 초기화
    node->pipe = NULL;
    node->heredoc_limiter = NULL;
    node->filename = NULL;
    node->dollar_vari = NULL;

    return node;
}

Pipeline *create_Pipeline(ASTNode *node)
{
    Pipeline *pipe;
    pipe = (Pipeline *)malloc(sizeof(Pipeline));
    if (!pipe)
    {
        log_errors("failed to allocate Pipeline pipe");
        return (NULL);
    }
    pipe->wstatus = 0;
    pipe->exitcode = -1;

    return (pipe);
}

//명시적으로 표현하여 추후에 더 read하기 쉽게
int set_pipe(ASTNode *node, Pipeline *pipe)
{
    node->pipe = create_Pipeline(node);
    if (!node->pipe) // 여기측은 더이상 errno받아와지지 않기때문에
        return (FAIL);
    return (SUCCESS);
}

int set_heredoc_limiter(ASTNode *node, char *limiter)
{
    node->heredoc_limiter = ft_strdup(limiter);
    if (!node->heredoc_limiter)
        return (log_errors("node->heredoc_limiter failed to allocated"));
    return (SUCCESS);
}

int set_filename(ASTNode *node, char *filename)
{
    node->filename = ft_strdup(filename);
    if (!node->filename)
        return (log_errors("node->filename failed to allocated"));
    return (SUCCESS);
}

int set_variable(ASTNode *node, char *variable_name)
{
    node->dollar_vari = ft_strdup(variable_name);
    if (!node->dollar_vari)
        return (log_errors("node->dollar_vari failed to allocated"));
    return (SUCCESS);
}

// ASTlist *create_ast_list()
// {
//     ASTlist *ast_list = malloc(sizeof(ASTlist));
//     if (!ast_list)
//     {
//         log_errors("Failed to allocate ASTlist node");
//         return (NULL);
//     }
//     ast_list->head = NULL;
//     ast_list->tail = NULL;
//     return (ast_list);
// }

// void add_ast_node(ASTlist *ast_list, ASTNode *new_node)
// {
//     if (ast_list->head == NULL) //head 설정 안되어있을시 tail & head에 첫번째 노드 연결
//     {
//         ast_list->head = new_node;
//         ast_list->tail = new_node;
//     }
//     else
//     {
//         ast_list->tail->right = new_node; // 이전 노드의 오른쪽에 새 노드를 연결
//         new_node->left = ast_list->tail; // 새노드에 왼쪽에 이전 노드 연결
//         ast_list->tail = new_node;        // 새 노드를 tail로 설정
//     }
// }
