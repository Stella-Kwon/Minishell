/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_next_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 04:33:16 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int reading_loop(t_op_next *next, char **buffer)
{
    while ((next->bytes_read = read(next->fd[0], *buffer + next->total_bytes, R_BUFFSIZE)) > 0)
    {
        next->total_bytes += next->bytes_read;
        if (next->total_bytes >= next->bufsize)
        {
            *buffer = ft_realloc_single(*buffer, next->total_bytes, &next->bufsize);
            if (!*buffer)
            {
                close(next->fd[0]);
                return (log_errors("Failed to realloc in read_from_child", ""));
            }
        }
    }
    return (SUCCESS);
}

int read_from_child(t_op_next *next, char **buffer)
{
    *buffer = malloc(R_BUFFSIZE);
    if (!*buffer)
        return (log_errors("Failed to malloc in read_from_child", ""));
    if (reading_loop(next, buffer) == FAIL)
        return (FAIL);
    if (next->bytes_read < 0)
    {
        close(next->fd[0]);
        return (log_errors("Failed to read from op_next_parents", ""));
    }
    (*buffer)[next->bytes_read] = '\0';
    close(next->fd[0]);
    return (SUCCESS);
}

int rm_quote_buffer(t_For_tokenize *tokenize, char **buffer, char **cp_input)
{
    char    *rm_quote_buffer;

    rm_quote_buffer = rm_quotes(*buffer);
    if (!rm_quote_buffer)
        return (log_errors("Failed to rm_quotes in rm_quote_buffer", ""));
    free_one((void **)buffer);
    if (join_inputs(cp_input, &rm_quote_buffer) == FAIL)
        return (FAIL);
    if (!*cp_input)
        return (log_errors("Failed to join_inputs in rm_quote_buffer", ""));
    if (tokenize->input)
        free_one((void **)&tokenize->input);
    tokenize->input = *cp_input;
    return (SUCCESS);
}

void initialize_op_next(t_op_next *next, t_For_tokenize *tokenize)
{
    next->status = 0;
    next->pid = 0;
    next->exitcode = 0;
    next->offset = tokenize->start - tokenize->input;
    next->bytes_read = 0;
    next->bufsize = R_BUFFSIZE;
    next->total_bytes = 0;
}

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