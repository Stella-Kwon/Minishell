/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 22:52:20 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int g_interrupt_signal;

int	node_command_without_cmd(t_ASTNode **node)
{
	if ((*node)->redir->infile == -1)
		return (print_error_redirect(&(*node)->command, \
									(*node)->redir->in_filename));
	if ((*node)->redir->outfile == -1)
		return (print_error_redirect(&(*node)->command, \
									(*node)->redir->out_filename));
	if ((*node)->redir->herestring_str)
	{
		if (here_string(&(*node)->redir) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

int	ast_node_execution(t_ASTNode **node)
{
	int	exitcode;

	if (node == NULL || *node == NULL)
		return (log_errors("AST node is NULL", ""));
	exitcode = heredoc_check(node);
	if (exitcode == FAIL)
		return (FAIL);
	if (exitcode == 130)
		return (130);
	if (exitcode == 3)
	{
		ft_putstr_fd("minishell: warning: unexpected end of here-document\n", 2);
		return (0);
	}
	if ((*node)->type == NODE_COMMAND && !(*node)->command)
	{
		exitcode = node_command_without_cmd(node);
		if (exitcode != SUCCESS)
			return (-1);
		else if (exitcode == SUCCESS)
			return (SUCCESS);
	}
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
	if (g_interrupt_signal == TRUE)
		(*node)->last_exitcode = 130;
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
