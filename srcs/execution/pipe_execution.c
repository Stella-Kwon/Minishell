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

static void	pipenode_left_exec_child(t_ASTNode **node, int *exitcode)
{
	(*node)->pipeline->left_pid = fork();
	if ((*node)->pipeline->left_pid == 0)
	{
		close((*node)->pipeline->fd[0]);
		if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) != SUCCESS)
		{
			(*node)->last_exitcode = EXIT_FAILURE;
			all_free((*node)->command->env);
			free_astnode((*node)->command->root_node);
			exit(EXIT_FAILURE);
		}
		*exitcode = heredoc_check(node);
		printf("exit : %d\n", *exitcode);
		if (*exitcode != SUCCESS)
			exit(*exitcode);
		if (!(*node)->command) // Check if command is NULL
			exit(node_command_without_cmd(node));
		*exitcode = ast_node_execution(&(*node)->left);
		(*node)->last_exitcode = *exitcode;
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
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
		{
			(*node)->last_exitcode = EXIT_FAILURE;
			all_free((*node)->command->env);
			free_astnode((*node)->command->root_node);
			exit(EXIT_FAILURE);
		}
		*exitcode = heredoc_check(node);
		printf("exit : %d\n", *exitcode);
		if (*exitcode != SUCCESS)
			exit(*exitcode);
		if (!(*node)->command) // Check if command is NULL
			exit(node_command_without_cmd(node));
		*exitcode = ast_node_execution(&(*node)->right);
		(*node)->last_exitcode = *exitcode;
		all_free((*node)->command->env);
		free_astnode((*node)->command->root_node);
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
<<<<<<< Updated upstream
	close((*node)->pipeline->fd[1]);
=======
	pipenode_right_exec_child(node, &exitcode);
	close((*node)->pipeline->fd[1]);
	close((*node)->pipeline->fd[0]);
	// if (waitpid((*node)->pipeline->left_pid, &status, 0) == -1)
	// {
	// 	(*node)->last_exitcode = waitpid_status(status);
	// 	return ((*node)->last_exitcode);
	// }
	// if (waitpid((*node)->pipeline->right_pid, &status, 0) == -1)
	// {
	// 	(*node)->last_exitcode = waitpid_status(status);
	// 	return ((*node)->last_exitcode);
	// }
	// (*node)->last_exitcode = waitpid_status(status);
	// return ((*node)->last_exitcode);
>>>>>>> Stashed changes
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
	if (g_interrupt_signal == TRUE)
		(*node)->command->exitcode = 0;
	// printf("exitcode : %d\n", (*node)->command->exitcode);
	return ((*node)->command->exitcode);
}


