/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 17:28:47 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ast_node_execution(t_ASTNode	**node)
{
	if (node == NULL || *node == NULL)
		return (log_errors("AST node is NULL", ""));
	if (heredoc_check(node) == FAIL)
		return (FAIL);
	if ((*node)->type == NODE_COMMAND)
		return (cmdnode_exec(node));
	if ((*node)->type == NODE_PIPE)
		return (pipenode_exec(node));
	if ((*node)->type == NODE_OR)
		return (ornode_exec(node));
	if ((*node)->type == NODE_AND)
		return (andnode_exec(node));
	return (SUCCESS);
}

int	cmdnode_exec(t_ASTNode	**node)
{
	int	last_exit_code;
	int	wstatus;

	last_exit_code = (*node)->last_exit_code;
	if (prepare_cmd(&(*node)->command, last_exit_code) == FAIL)
		return (FAIL);
	if (builtin_filesystem((*node)->command) == SUCCESS)
	{
		if (common_pre_child(&(*node)->redir) == FAIL)
			return (FAIL);
		(*node)->last_exit_code = 0;
		return (SUCCESS);
	}
	(*node)->pipeline->pid = fork();
    if ((*node)->pipeline->pid == -1)
        return (log_errors("Failed to fork in cmdnode_exec", ""));
    if ((*node)->pipeline->pid == 0) 
	{
        if (common_pre_child(&(*node)->redir) == FAIL)
            exit(EXIT_FAILURE);
        exit(action_child(&(*node)->command, &(*node)->redir));
	}
    if (waitpid((*node)->pipeline->pid, &wstatus, 0) == -1)
	{
		return (log_errors("waitpid failed", ""));
	}
	last_exit_code = waitpid_status(wstatus);
    (*node)->last_exit_code = last_exit_code; // 마지막 종료 코드 저장
    return (SUCCESS);
}


int	andnode_exec(t_ASTNode	**node)
{
	if ((*node)->left && ast_node_execution(&(*node)->left) == SUCCESS)
	{
		if ((*node)->right)
			return (cmdnode_exec(&(*node)->right));
	}
	return (SUCCESS);
}

int	ornode_exec(t_ASTNode	**node)
{
	if ((*node)->left && ast_node_execution(&(*node)->left) != SUCCESS)
	{
		if ((*node)->right)
			return (cmdnode_exec(&(*node)->right));
	}
	return (SUCCESS);
}
