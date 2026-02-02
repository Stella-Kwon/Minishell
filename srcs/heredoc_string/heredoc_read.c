/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:00:00 by skwon2            #+#    #+#             */
/*   Updated: 2024/01/30 00:00:00 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int g_heredoc_interrupted = FALSE;

char **read_heredoc_body(char *limiter, char **env, int last_exit_code)
{
    char **lines;
    char *line;
    char *clean_limiter;
    int count;
    int bufsize;
    int should_expand;

    should_expand = !is_quoted_string(limiter);
    clean_limiter = rm_quotes(limiter);
    if (!clean_limiter)
        return (NULL);
    lines = malloc(sizeof(char *) * 10);
    if (!lines)
        return (free(clean_limiter), NULL);
    count = 0;
    bufsize = 10;
    g_heredoc_interrupted = FALSE;
    while (1)
    {
        line = read_line_safe("> ");
        if (!line || line == (char *)-1)
        {
            if (line == (char *)-1)
                g_heredoc_interrupted = TRUE;
            free(clean_limiter);
            while (count > 0)
                free(lines[--count]);
            free(lines);
            return (NULL);
        }
        if (ft_strcmp(line, clean_limiter) == 0)
        {
            free_one((void **)&line);
            break;
        }
        if (should_expand && find_dollar_signs(&line, env, last_exit_code) == FAIL)
        {
            free_one((void **)&line);
            free(clean_limiter);
            while (count > 0)
                free(lines[--count]);
            free(lines);
            return (NULL);
        }
        lines[count++] = line;
        lines = ft_realloc_double(lines, count + 1, &bufsize);
        if (!lines)
            return (free(clean_limiter), NULL);
    }
    lines[count] = NULL;
    free(clean_limiter);
    return (lines);
}

int read_all_heredocs(t_ASTNode **node, char **env, int last_exit_code)
{
    int i;

    if (!(*node)->redir->heredoc_limiter)
        return (SUCCESS);
    (*node)->redir->heredoc_body = malloc(sizeof(char **) *
                                          (*node)->redir->heredoc_i);
    if (!(*node)->redir->heredoc_body)
        return (log_errors("Failed to allocate heredoc_body", ""));
    i = 0;
    while (i < (*node)->redir->heredoc_i)
    {
        (*node)->redir->heredoc_body[i] =
            read_heredoc_body((*node)->redir->heredoc_limiter[i],
                              env, last_exit_code);
        if (!(*node)->redir->heredoc_body[i])
        {
            if (g_heredoc_interrupted == TRUE)
            {
                g_heredoc_interrupted = FALSE;
                return (1); // Ctrl+C
            }
            return (SUCCESS); // EOF (Ctrl+D) - normal exit
        }
        i++;
    }
    return (SUCCESS);
}
