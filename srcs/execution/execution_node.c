/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 00:32:53 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	node_command_without_cmd(t_ASTNode **node)
// {
// 	if ((*node)->redir->infile == -1)
// 		return (print_error_redir(&(*node)->command,
// 									(*node)->redir->in_filename,
// 									(*node)->redir->errno_in));
// 	if ((*node)->redir->outfile == -1)
// 		return (print_error_redir(&(*node)->command,
// 									(*node)->redir->out_filename,
// 									(*node)->redir->errno_out));
// 	if ((*node)->redir->herestring_str)
// 	{
// 		if (here_string(&(*node)->redir) != SUCCESS)
// 		{
// 			return (FAIL);
// 		}
// 	}
// 	return (SUCCESS);
// }

// int child_heredoc_exec(t_ASTNode **node)
// {
// 	int exitcode;

// 	exitcode = 0;
// 	exitcode = check_heredoc(node, exitcode);
// 	if (exitcode != SUCCESS)
// 		return (exitcode);
// 	if ((*node)->type == NODE_COMMAND && !(*node)->command)
// 	{
// 		exitcode = node_command_without_cmd(node);
// 		// printf("exitcode : %d\n", exitcode);
// 		return (exitcode);
// 		// if (exitcode != SUCCESS)
// 		// 	return (-1);
// 		// else if (exitcode == SUCCESS)
// 		// 	return (SUCCESS);
// 	}
// 	return (SUCCESS);
// }

// int heredoc_exec(t_ASTNode **node)
// {
// 	pid_t heredoc_pid;
// 	int status;

// 	if ((*node)->redir->heredoc_limiter)
// 	{
// 		heredoc_pid = fork();
// 		if (heredoc_pid == -1)
// 			return log_errors("Failed to fork for heredoc", "");
// 		if (heredoc_pid == 0)
// 			exit(heredoc_check(node));
// 		else
// 		{
// 			waitpid(heredoc_pid, &status, 0);
// 			(*node)->last_exitcode = waitpid_status(status);
// 		}
// 	}
// 	return ((*node)->last_exitcode);
// }



int	ast_node_execution(t_ASTNode **node)
{
	if ((*node)->type == NODE_PIPE)
		return (pipenode_exec(node));
	if (node == NULL || *node == NULL)
		return (log_errors("AST node is NULL", ""));
	int exitcode = heredoc_check(node);
	printf("exit : %d\n", exitcode);
	if (exitcode != SUCCESS)
		return exitcode;
	if (!(*node)->command) // Check if command is NULL
		return node_command_without_cmd(node);
	if ((*node)->type == NODE_COMMAND)
		return (cmdnode_exec(node));
	if ((*node)->type == NODE_OR)
		return (ornode_exec(node));
	if ((*node)->type == NODE_AND)
		return (andnode_exec(node));
	return (SUCCESS);
}

// int	cmdnode_exec(t_ASTNode	**node)
// {
// 	int exitcode;

// 	signal_set_exec();
// 	exitcode = heredoc_exec(node);
// 	printf("exit : %d\n", exitcode);
// 	if (exitcode != SUCCESS)
// 		return (exitcode);
// 	if ((*node)->type == NODE_COMMAND && !(*node)->command)
// 	{
// 		exitcode = node_command_without_cmd(node);
// 		// printf("exitcode : %d\n", exitcode);
// 		return (exitcode);
// 		// if (exitcode != SUCCESS)
// 		// 	return (-1);
// 		// else if (exitcode == SUCCESS)
// 		// 	return (SUCCESS);
// 	}
// 	(*node)->command->exitcode = (*node)->last_exitcode;
// 	if (prepare_cmd(&(*node)->command, (*node)->last_exitcode) == FAIL)
// 		return (FAIL);
// 	if (check_builtin((*node)->command) == TRUE)
// 		return (action_builtin(&(*node)->command, &(*node)->redir));
// 	if (find_command_path(&(*node)->command) != SUCCESS)
// 		return ((*node)->command->exitcode);
// 	(*node)->pipeline->pid = fork();
// 	if ((*node)->pipeline->pid == -1)
// 		return (log_errors("Failed to fork in cmdnode_exec", ""));
// 	if ((*node)->pipeline->pid == 0)
// 	{
// 		exit(action_child(&(*node)->command, &(*node)->redir));
// 	}
// 	return (action_parents(&(*node)->redir, &(*node)->pipeline,
// 								&(*node)->command));
// }

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

int node_command_without_cmd(t_ASTNode **node)
{
	if ((*node)->redir->infile == -1)
		return print_error_redir(&(*node)->command, (*node)->redir->in_filename, (*node)->redir->errno_in);
	if ((*node)->redir->outfile == -1)
		return print_error_redir(&(*node)->command, (*node)->redir->out_filename, (*node)->redir->errno_out);
	if ((*node)->redir->herestring_str && here_string(&(*node)->redir) != SUCCESS)
		return FAIL;
	return SUCCESS;
}

// int heredoc_exec(t_ASTNode **node)
// {
// 	if (!(*node)->redir->heredoc_limiter)
// 		return SUCCESS;

// 	pid_t heredoc_pid = fork();
// 	if (heredoc_pid == -1)
// 		return log_errors("Failed to fork for heredoc", "");

// 	if (heredoc_pid == 0)
// 		exit(heredoc_check(node));

// 	int status;
// 	waitpid(heredoc_pid, &status, 0);
// 	(*node)->last_exitcode = waitpid_status(status);
// 	return (*node)->last_exitcode;
// }

int cmdnode_exec(t_ASTNode **node)
{
	signal_set_exec();
	(*node)->command->exitcode = (*node)->last_exitcode;
	if (prepare_cmd(&(*node)->command, (*node)->last_exitcode) == FAIL)
		return FAIL;

	if (check_builtin((*node)->command) == TRUE)
		return action_builtin(&(*node)->command, &(*node)->redir);

	if (find_command_path(&(*node)->command) != SUCCESS)
		return (*node)->command->exitcode;

	(*node)->pipeline->pid = fork();
	if ((*node)->pipeline->pid == -1)
		return log_errors("Failed to fork in cmdnode_exec", "");

	if ((*node)->pipeline->pid == 0)
		exit(action_child(&(*node)->command, &(*node)->redir));

	return action_parents(&(*node)->redir, &(*node)->pipeline, &(*node)->command);
}
