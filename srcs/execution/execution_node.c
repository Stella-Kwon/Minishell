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

int	heredoc_check(ASTNode **node)
{
    if ((*node)->redir->heredoc_limiter)
    {
        if (here_doc(node) == FAIL)
            return (log_errors("Failed here_doc in heredoc_check", ""));
    }
    return (SUCCESS);
}

int ast_node_execution(ASTNode **node)
{
    if (node == NULL || *node == NULL)
        return (log_errors("AST node is NULL", ""));

    if (heredoc_check(node) == FAIL)
        return (FAIL);

    if (pipe((*node)->pipeline->fd) == -1)
        return (log_errors("Failed to create pipe", ""));

    (*node)->pipeline->tmp_fd = dup(STDIN_FILENO);
    if ((*node)->pipeline->tmp_fd == -1)
        return (log_errors("Failed to backup stdin", ""));

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
    if ((*node)->type == NODE_COMMAND)
        return (CMDnode_exec(node));

    return (SUCCESS);
}

int ANDnode_exec(ASTNode **node)
{
    int pid;

    pid = fork();
    if (pid == -1)
        return (log_errors("Failed to fork in ANDnode_exec", ""));
    if ((*node)->left && CMDnode_exec(&(*node)->left) == SUCCESS)
    {
        if ((*node)->right)
        {
			if (ast_node_execution(&(*node)->right) != SUCCESS)
                return (FAIL);
    	}
    }
	return (SUCCESS);
}

int ORnode_exec(ASTNode **node)
{
    int pid;

    pid = fork();
    if (pid == -1)
        return (log_errors("Failed to fork in ORode_exec", ""));
    if ((*node)->left && CMDnode_exec(&(*node)->left) != SUCCESS)
	{
		if ((*node)->right)
		{
			if (ast_node_execution(&(*node)->right) != SUCCESS)
				return (FAIL);
		}
    }
    return (SUCCESS);
}

int CMDnode_exec(ASTNode **node)
{
    int pid;
    
    pid = fork();
    if (pid == -1)
        return (log_errors("Failed to fork in cmdnode_exec", ""));
    if ((*node)->left || (*node)->right)
        return (log_errors("Something has been allocated on left or right in the COMMAND_NODE", ""));
    if (pid == 0)
    {
        if (action_child((*node)->command, (*node)->redir, (*node)->pipeline) != SUCCESS)
            exit ((*node)->command->exitcode);
    }
    else
    {
		(*node)->pipeline->pid = pid;
        if (action_parents((*node)->command, (*node)->pipeline) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    return (SUCCESS);
}
