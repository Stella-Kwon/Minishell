/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_next.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 22:30:20 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int child_exec(int fd)
// {
// 	char	*new_input;

// 	while (1)
// 	{
// 		new_input = readline("> ");
// 		if (!new_input)
// 			exit(3);
// 		if (ft_strcmp(new_input, "") == 0)
// 		{
// 			free_one((void **)&new_input);
// 			continue;
// 		}
// 		if (write(fd, new_input, ft_strlen(new_input)) == -1)
// 		{
// 			free_one((void **)&new_input);
// 			exit(log_errors("Failed to write in child_exec", ""));
// 		}
// 		free_one((void **)&new_input);
// 		break;
// 	}
// 	close(fd);
// 	exit(SUCCESS);
// }

// int read_from_child(int fd, char **buffer)
// {
// 	ssize_t bytes_read;

// 	*buffer = malloc(R_BUFFSIZE);
// 	if (!*buffer)
// 		return (log_errors("Failed to malloc in read_from_child", ""));

// 	bytes_read = read(fd, *buffer, R_BUFFSIZE);
// 	if (bytes_read < 0)
// 	{
// 		free_one((void **)buffer);
// 		return (log_errors("Failed to read from child", ""));
// 	}
// 	(*buffer)[bytes_read] = '\0';
// 	close(fd);
// 	return (SUCCESS);
// }

// int op_next_child(t_op_next *next)
// {
// 	next->pid = fork();
// 	if (next->pid == -1)
// 	{
// 		return (log_errors("Failed to fork in op_next_child", ""));
// 	}
// 	if (next->pid == 0)
// 	{
// 		signal_set(SIG_DFL, SIG_IGN);
// 		child_exec(next->fd);
// 	}
// 	return (SUCCESS);
// }

// int check_operation_next(t_For_tokenize *tokenize)
// {
// 	t_op_next next;
// 	char *cpy_input;
// 	char *buffer;

// 	initialize_op_next(&next, tokenize);
// 	next.fd = open(".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (next.fd == -1)
// 		return log_errors(".tmp", "Failed to open file in check_operation_next");
// 	if (op_next_child(&next) == FAIL)
// 		return (FAIL);
// 	if (waitpid(next.pid, &next.status, 0) == -1)
// 		return (waitpid_status(next.status));
// 	close(next.fd);
// 	next.fd = open(".tmp", O_RDONLY);
// 	if (next.fd == -1)
// 		return (log_errors(".tmp", "Failed to open file in check_op_next"));
// 	if (unlink(".tmp") == -1)
// 		return (log_errors("Failed to unlink in check_op_next", ""));
// 	if (read_from_child(&next, &buffer) == FAIL)
// 		return (FAIL);
// 	printf("read : %s\n", buffer);
// 	cpy_input = ft_strdup(tokenize->input);
// 	if (!cpy_input)
// 		return (log_errors("Failed to ft_strdup from op_next_parents", ""));
// 	if (join_inputs(&tokenize->input, &buffer) == FAIL)
// 		return (FAIL);
// 	add_history(tokenize->input);
// 	if (rm_quote_buffer(tokenize, &buffer, &cpy_input) == FAIL)
// 		return (FAIL);
// 	tokenize->start = tokenize->input + next.offset;
// 	next.exitcode = waitpid_status(next.status);

// 	free_one((void **)&buffer); // 사용한 버퍼 메모리 해제
// 	return (SUCCESS);
// }

int join_inputs(char **tokenize_input, char **new_input)
{
    char *tmp;

    tmp = ft_strjoin(" ", *new_input);
    free_one((void **)new_input);
    if (!tmp)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }
    *new_input = ft_strjoin(*tokenize_input, tmp);
	free_one((void **)tokenize_input);
    free_one((void **)&tmp);
    if (!*new_input)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }
    *tokenize_input = *new_input;
    return (SUCCESS);
}

int check_operation_next(t_For_tokenize *tokenize)
{
	char *new_input;
	ptrdiff_t offset;
	char *cpy_input;
	char *cpy_new_input;


	offset = tokenize->start - tokenize->input;
	signal_set(SIG_DFL, SIG_IGN, UNSET);
	new_input = readline("> ");
	signal_set(SIG_DFL, SIG_DFL, SET);
	if (g_received_signal == 10)
		return (FAIL);
	if (!new_input)
		return (3);
	cpy_input = ft_strdup(tokenize->input);
	if (!cpy_input)
		return (log_errors("Failed to strdup:tok from op_next_parents", ""));
	cpy_new_input = ft_strdup(new_input);
	if (!cpy_new_input)
		return (log_errors("Failed to strdup:new from op_next_parents", ""));
	if (join_inputs(&tokenize->input, &new_input) == FAIL)
		return (FAIL);
	add_history(tokenize->input);
	if (rm_quote_buffer(tokenize, &cpy_new_input, &cpy_input) == FAIL)
		return (FAIL);
	tokenize->start = tokenize->input + offset;
	return (SUCCESS);
}