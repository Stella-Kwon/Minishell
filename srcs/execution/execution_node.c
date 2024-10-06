/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 19:04:06 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_check(ASTNode **node)
{
    if ((*node)->redir->heredoc_limiter)
    {
        if (here_doc(node) == FAIL)
            return (log_errors("Failed here_doc in heredoc_check", ""));
        if ((*node)->redir->tmp_infile != -2)
            (*node)->redir->infile = (*node)->redir->tmp_infile;
    }
    return (SUCCESS);
}

int ast_node_execution(ASTNode **node)
{
    if (node == NULL || *node == NULL)
        return (log_errors("AST node is NULL", ""));
    if ((*node)->type == NODE_PIPE)
        return (PIPEnode_exec(node));
    if (heredoc_check(node) == FAIL)
        return (FAIL);
    if ((*node)->type == NODE_COMMAND)
        return (CMDnode_exec(node));
    return (execute_node(node));
}

int execute_node(ASTNode **node)
{
    if ((*node)->type == NODE_PIPE)
        return (PIPEnode_exec(node));
    if ((*node)->type == NODE_OR)
        return (ORnode_exec(node));
    if ((*node)->type == NODE_AND)
        return (ANDnode_exec(node));
    return (SUCCESS);
}

int ANDnode_exec(ASTNode **node)
{
    // 왼쪽 노드 실행
    if ((*node)->left && ast_node_execution(&(*node)->left) == SUCCESS)
    {
        // 왼쪽 노드가 성공하면 오른쪽 노드 실행
        printf("\nwent into rightnode : %s\n\n", (*node)->right->command->cmd);
        if ((*node)->right)
                return (CMDnode_exec(&(*node)->right)); // 오른쪽 노드가 실패한 경우 종료
    }
    return (SUCCESS); // 성공적으로 실행된 경우
}


int ORnode_exec(ASTNode **node)
{
    // 왼쪽 노드 실행
    if ((*node)->left && ast_node_execution(&(*node)->left) != SUCCESS)
    {
        // 왼쪽 노드가 실패하면 오른쪽 노드 실행
        printf("\nwent into rightnode : %s\n\n", (*node)->right->command->cmd);
        if ((*node)->right)
            return (CMDnode_exec(&(*node)->right)); // 오른쪽 노드가 실패한 경우 종료
    }
    return (SUCCESS); // 성공적으로 실행된 경우
}

int CMDnode_exec(ASTNode **node)
{
    (*node)->pipeline->pid = fork();
    if ((*node)->pipeline->pid == -1)
        return (log_errors("Failed to fork in cmdnode_exec", ""));
    if ((*node)->left || (*node)->right)
        return (log_errors("Something has been allocated on left or right in the COMMAND_NODE", ""));
    //여기에 프린트 넣엇다고 두번 돌아가는 이상한...상황.. ㅋ
    if ((*node)->pipeline->pid == 0)
        exit(action_child(&(*node)->command, &(*node)->redir, &(*node)->pipeline));
    return (action_parents(&(*node)->redir, &(*node)->pipeline));
}
