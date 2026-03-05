/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by skwon2            #+#    #+#             */
/*   Updated: 2026/03/05 11:20:36 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void pipenode_left_exec_child(t_ASTNode **node, int *exitcode)
{
	(*node)->pipeline->left_pid = fork();
	if ((*node)->pipeline->left_pid == 0)
	{
		close((*node)->pipeline->fd[0]);
		if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) != SUCCESS)
		{
			all_free((*node)->command->env);
			free_astnode((*node)->command->root_node);
			exit(EXIT_FAILURE);
		}
		*exitcode = ast_node_execution(&(*node)->left);
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
		exit(*exitcode);
	}
	if ((*node)->pipeline->left_pid > 0)
		child_spawned();
}

static void pipenode_right_exec_child(t_ASTNode **node, int *exitcode)
{
	(*node)->pipeline->right_pid = fork();
	if ((*node)->pipeline->right_pid == 0)
	{
		close((*node)->pipeline->fd[1]);
		if (dup_and_close((*node)->pipeline->fd[0], STDIN_FILENO) != SUCCESS)
		{
			(*node)->command->exitcode = EXIT_FAILURE;
			all_free((*node)->command->env);
			free_astnode((*node)->command->root_node);
			exit(EXIT_FAILURE);
		}
		*exitcode = ast_node_execution(&(*node)->right);
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
		exit(*exitcode);
	}
	if ((*node)->pipeline->right_pid > 0)
		child_spawned();
}

static int pipenode_exec_normal(t_ASTNode **node)
{
	int status;
	int exitcode;

	exitcode = 0;
	if (pipe((*node)->pipeline->fd) == -1)
		return (log_errors("Failed to create pipe", strerror(errno)));
	pipenode_left_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[1]);
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[0]);
	if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
		return (log_errors("Failed to wait left_pipe in pipenode_exec_normal", strerror(errno)));
	child_reaped();
	if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
		return (log_errors("Failed to wait right_pipe in pipenode_exec_normal", strerror(errno)));
	child_reaped();
	(*node)->command->exitcode = waitpid_status(status);
	return ((*node)->command->exitcode);
}

static int pipenode_exec_heredoc(t_ASTNode **node)
{
	int status;
	int exitcode;

	exitcode = 0;
	if (pipe((*node)->pipeline->fd) == -1)
		return (log_errors("Failed to create pipe", strerror(errno)));
	pipenode_left_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[1]);
	if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
		return (log_errors("Failed to wait left_pipe in pipenode_exec_heredoc", strerror(errno)));
	child_reaped();
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[0]);
	if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
		return (log_errors("Failed to wait right_pipe in pipenode_exec_heredoc", strerror(errno)));
	child_reaped();
	(*node)->command->exitcode = waitpid_status(status);
	return ((*node)->command->exitcode);
}

int pipenode_exec(t_ASTNode **node)
{
	if ((*node)->left->redir && (*node)->left->redir->heredoc_limiter != NULL)
		return (pipenode_exec_heredoc(node));
	else
		return (pipenode_exec_normal(node));
}
