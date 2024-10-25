/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 00:41:19 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void pipenode_left_exec_child(t_ASTNode **node, int *exitcode,
									 int redirect)
{
	(*node)->pipeline->left_pid = fork();
	if ((*node)->pipeline->left_pid == 0)
	{
		close((*node)->pipeline->fd[0]);
		if (redirect == FALSE)
			close((*node)->pipeline->fd[1]);
		if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) != SUCCESS)
		{
			(*node)->command->exitcode = EXIT_FAILURE;
			all_free((*node)->command->env);
			free_astnode((*node)->command->root_node);
			exit(EXIT_FAILURE);
		}
		*exitcode = ast_node_execution(&(*node)->left);
		(*node)->command->exitcode = *exitcode;
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
		exit(*exitcode);
	}
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
		(*node)->command->exitcode = *exitcode;
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
		exit(*exitcode);
	}
}

static int pipenode_exec_normal(t_ASTNode **node)
{
	int status;
	int exitcode;

	exitcode = 0;
	if (pipe((*node)->pipeline->fd) == -1)
		return (log_errors("Failed to create pipe", strerror(errno)));
	pipenode_left_exec_child(node, &exitcode, TRUE);
	close((*node)->pipeline->fd[1]);
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[0]);
	if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
	{
		(*node)->command->exitcode = waitpid_status(status);
		return ((*node)->command->exitcode);
	}
	if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
	{
		(*node)->command->exitcode = waitpid_status(status);
		return ((*node)->command->exitcode);
	}
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
	pipenode_left_exec_child(node, &exitcode, TRUE);
	close((*node)->pipeline->fd[1]);
	if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
	{
		(*node)->command->exitcode = waitpid_status(status);
		return ((*node)->command->exitcode);
	}
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[0]);
	if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
	{
		(*node)->command->exitcode = waitpid_status(status);
		return ((*node)->command->exitcode);
	}
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