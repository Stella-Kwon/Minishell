/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:03:46 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	pipenode_left_exec_child(t_ASTNode **node, int *exitcode)
{
	(*node)->pipeline->left_pid = fork();
	if ((*node)->pipeline->left_pid == 0)
	{
		close((*node)->pipeline->fd[0]);
		if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) != SUCCESS)
		{
			exit(EXIT_FAILURE);
		}
		*exitcode = ast_node_execution(&(*node)->left);
		(*node)->last_exitcode = *exitcode;
		exit(*exitcode);
	}
}

static void	pipenode_right_exec_child(t_ASTNode **node, int *exitcode)
{
	(*node)->pipeline->right_pid = fork();
	if ((*node)->pipeline->right_pid == 0)
	{
		close((*node)->pipeline->fd[1]);
		if (dup_and_close((*node)->pipeline->fd[0], STDIN_FILENO) != SUCCESS)
			exit(EXIT_FAILURE);
		*exitcode = ast_node_execution(&(*node)->right);
		(*node)->last_exitcode = *exitcode;
		exit(*exitcode);
	}
}

int	pipenode_exec(t_ASTNode **node)
{
	int	status;
	int	exitcode;

	exitcode = 0;
	if (pipe((*node)->pipeline->fd) == -1)
		return (log_errors("Failed to create pipe", strerror(errno)));
	pipenode_left_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[1]);
	if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
	{
		(*node)->last_exitcode = waitpid_status(status);
		return ((*node)->last_exitcode);
	}
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[0]);
	if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
	{
		(*node)->last_exitcode = waitpid_status(status);
		return ((*node)->last_exitcode);
	}
	(*node)->last_exitcode = waitpid_status(status);
	return ((*node)->last_exitcode);
}
