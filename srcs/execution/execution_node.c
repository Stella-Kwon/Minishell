/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:37:19 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	node_command_without_cmd(t_ASTNode **node)
{
	if ((*node)->redir->infile == -1)
		return (print_error_redir(&(*node)->command, \
		(*node)->redir->in_filename, (*node)->redir->errno_in));
	if ((*node)->redir->outfile == -1)
		return (print_error_redir(&(*node)->command, \
		(*node)->redir->out_filename, (*node)->redir->errno_out));
	if ((*node)->redir->herestring_str)
	{
		if (here_string(&(*node)->redir) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

int	cmdnode_exec(t_ASTNode **node)
{
	signal_set_exec();
	if (g_interrupt_signal == TRUE)
		g_interrupt_signal = FALSE;
	(*node)->command->exitcode = (*node)->last_exitcode;
	if (prepare_cmd(&(*node)->command, (*node)->last_exitcode) == FAIL)
		return (FAIL);
	if (check_builtin((*node)->command) == TRUE)
		return (action_builtin(&(*node)->command, &(*node)->redir));
	if (find_command_path(&(*node)->command) != SUCCESS)
		return ((*node)->command->exitcode);
	(*node)->pipeline->pid = fork();
	if ((*node)->pipeline->pid == -1)
		return (log_errors("Failed to fork in cmdnode_exec", ""));
	if ((*node)->pipeline->pid == 0)
	{
		exit(action_child(&(*node)->command, &(*node)->redir));
	}
	return (action_parents(&(*node)->redir, &(*node)->pipeline, \
							&(*node)->command));
}

int	ast_node_execution(t_ASTNode **node)
{
	if (node == NULL || *node == NULL)
		return (log_errors("AST node is NULL", ""));
	if ((*node)->redir && (*node)-> redir->heredoc_limiter)
		return (heredoc_exec(node));
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

int	andnode_exec(t_ASTNode	**node)
{
	if ((*node)->left && ast_node_execution(&(*node)->left) == SUCCESS)
	{
		if ((*node)->right)
			return (ast_node_execution(&(*node)->right));
	}
	return (SUCCESS);
}

int	ornode_exec(t_ASTNode	**node)
{
	if ((*node)->left && ast_node_execution(&(*node)->left) != SUCCESS)
	{
		if ((*node)->right)
			return (ast_node_execution(&(*node)->right));
	}
	return (SUCCESS);
}
