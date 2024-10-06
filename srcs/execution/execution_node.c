/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:24:33 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ast_node_execution(t_ASTNode	**node)
{
	if (node == NULL || *node == NULL)
		return (log_errors("AST node is NULL", ""));
	if ((*node)->type == NODE_PIPE)
		return (pipenode_exec(node));
	if (heredoc_check(node) == FAIL)
		return (FAIL);
	if ((*node)->type == NODE_COMMAND)
		return (cmdnode_exec(node));
	return (execute_node(node));
}

int	execute_node(t_ASTNode	**node)
{
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
	(*node)->pipeline->pid = fork();
	if ((*node)->pipeline->pid == -1)
		return (log_errors("Failed to fork in cmdnode_exec", ""));
	if ((*node)->left || (*node)->right)
		return (log_errors("Something has been allocated \
		on left or right in the COMMAND_NODE", ""));
	if ((*node)->pipeline->pid == 0)
		exit(action_child(&(*node)->command, &(*node)->redir));
	return (action_parents(&(*node)->redir, &(*node)->pipeline));
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
