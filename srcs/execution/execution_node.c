/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 14:46:43 by suminkwon        ###   ########.fr       */
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
	int	last_exitcode;
	int	wstatus;

	last_exitcode = (*node)->last_exitcode;
	if (prepare_cmd(&(*node)->command, last_exitcode) == FAIL)
		return (FAIL);
	if (builtin_filesystem((*node)->command) == SUCCESS)
	{
		if (common_pre_child(&(*node)->redir) == FAIL)
			return (FAIL);
		(*node)->last_exitcode = 0;
		return (SUCCESS);
	}
	(*node)->pipeline->pid = fork();
    if ((*node)->pipeline->pid == -1)
        return (log_errors("Failed to fork in cmdnode_exec", ""));
    if ((*node)->pipeline->pid == 0) 
        exit(action_child(&(*node)->command, &(*node)->redir));
    if (waitpid((*node)->pipeline->pid, &wstatus, 0) == -1)
        return (log_errors("waitpid failed", ""));
	(*node)->command->exitcode = waitpid_status(wstatus);
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
