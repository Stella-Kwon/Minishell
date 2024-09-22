/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 21:17:46 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_check(ASTNode **node)
{
    if ((*node)->command->heredoc_limiter)
    {
        if (here_doc(node) == FAIL)
            return (log_errors("Failed here_doc in heredoc_check"));
    }
    return (SUCCESS);
}

int *ast_node_execution(ASTNode **node)
{
    if (heredoc_check(node) == FAIL)
        return (NULL);
    if (pipe((*node)->pipeline->fd) == -1) // 어차피 
        return (log_errors("Failed to pipe in execute_command"));
    (*node)->pipeline->tmp_fd = dup(STDIN_FILENO); // defaul it as stdin_fileno.
    if ((*node)->pipeline->tmp_fd == -1)
    {
        log_errors("Failed dup in ast_node_execution");
        return (NULL);
    }
    if ((*node)->type == NODE_PIPE)
    {
        if (PIPEnode_exec(node) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else if ((*node)->type == NODE_OR)
    {
        if (ORnode_exec(node) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else if ((*node)->type == NODE_AND)
    {
        if (ANDnode_exec(node) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else if ((*node)->type == NODE_SUBSHELL)
    {
        
    }
    else if ((*node)->type == NODE_COMMAND)
    {
        if (CMDnode_exec(node) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    return (SUCCESS);
}

int ANDnode_exec(ASTNode **node)
{
    int pid;

    pid = fork();
    if (!pid)
        return (log_errors("Failed to fork in cmdnode_exec"));
    if ((*node)->left) // pipe용 따로 해줘야함.ㅎ 왜냐면 구현할때 parents부분이 달라지기때문에.
    {
        if (CMDnode_exec((*node)->left) == SUCCESS)
        {
            if ((*node)->right)
            {
                if (ast_node_execution((*node)->right) != SUCCESS)
                    return (FAIL);
            }
        }
    }
    return (SUCCESS);
}

int ORnode_exec(ASTNode **node)
{
    int pid;

    pid = fork();
    if (!pid)
        return (log_errors("Failed to fork in cmdnode_exec"));
    if ((*node)->left) // pipe용 따로 해줘야함.ㅎ 왜냐면 구현할때 parents부분이 달라지기때문에.
    {
        if (CMDnode_exec((*node)->left) != SUCCESS)
        {
            if ((*node)->right)
            {
                if (ast_node_execution((*node)->right) != SUCCESS)
                    return (FAIL);
            }
        }
    }
    return (SUCCESS);
}

int CMDnode_exec(ASTNode **node)
{
    int pid;
    
    pid = fork();
    if (!pid)
        return (log_errors("Failed to fork in cmdnode_exec"));
    if ((*node)->left || (*node)->right)
        return (log_errors("Something has been allocated on left or right in the COMMAND_NODE"));
    if (pid == 0)
    {
        if (action_child((*node)->command, (*node)->pipeline) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else
    {
        if (action_parents((*node)->command, (*node)->pipeline) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    return (SUCCESS);
}