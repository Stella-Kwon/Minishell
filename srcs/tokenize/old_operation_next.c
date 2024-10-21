/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_next.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 04:33:16 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int child_exec(int write_fd)
{
	char *new_input;

	while (1)
	{
		new_input = readline("> ");
		if (!new_input)
			exit(3);
		if (ft_strcmp(new_input, "") == 0)
		{
			free_one((void **)&new_input);
			continue;
		}
		// if (write(write_fd, new_input, ft_strlen(new_input)) == -1)
		// {
		// 	free_one((void **)&new_input);
		// 	exit(log_errors("Failed to write in child_exec", ""));
		// }
		free_one((void **)&new_input);
		break;
	}
	close(write_fd);
	exit(SUCCESS);
}

int op_next_parents(t_op_next *next, t_For_tokenize *tokenize)
{
	char	*buffer;
	char	*cpy_input;

	close(next->fd[1]);
	if (waitpid(next->pid, &next->status, 0) == -1)
	{
		next->exitcode = waitpid_status(next->status);
		return (next->exitcode);
	}
	if (read_from_child(next, &buffer) == FAIL)
		return (FAIL);
	

	cpy_input = ft_strdup(tokenize->input);
	if (!cpy_input)
		return (log_errors("Failed to ft_strdup from op_next_parents", ""));
	if (join_inputs(&tokenize->input, &buffer) == FAIL)
		return (FAIL);
	add_history(tokenize->input);
	if (rm_quote_buffer(tokenize, &buffer, &cpy_input) == FAIL)
		return (FAIL);
	tokenize->start = tokenize->input + next->offset;
	next->exitcode = waitpid_status(next->status);
	return (next->exitcode);
}

int op_next_child(t_op_next *next)
{
	next->pid = fork();
	if (next->pid == -1)
	{
		close(next->fd[0]);
		close(next->fd[1]);
		return (log_errors("Failed to fork in opt_next_child", ""));
	}
	if (next->pid == 0)
	{
		close(next->fd[0]);
		// readline_signal();
		signal(SIGINT, SIG_IGN);
		child_exec(next->fd[1]);
	}
	return (FAIL);
}

	int check_operation_next(t_For_tokenize *tokenize)
{
	t_op_next next;

	initialize_op_next(&next, tokenize);
	if (pipe(next.fd) == -1)
		return (log_errors("Failed to create pipe in op_next", ""));
	if (op_next_child(&next) == FAIL)
		return (FAIL);
	if (op_next_parents(&next, tokenize) == FAIL)
		return (FAIL);
	if (next.exitcode == 3)
		return (3);
	return (SUCCESS);
}