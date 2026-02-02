/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 00:00:00 by skwon2            #+#    #+#             */
/*   Updated: 2024/01/30 00:00:00 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char *read_line_tty(const char *prompt)
{
    char *line;

    g_interrupt_signal = FALSE;
    g_readline_jmp_active = TRUE;
    signal_set_rl();
    if (sigsetjmp(g_readline_jmp_buf, 1) != 0)
    {
        g_readline_jmp_active = FALSE;
        signal_setup();
        return ((char *)-1);
    }
    if (prompt)
        line = readline(prompt);
    else
        line = readline("");
    g_readline_jmp_active = FALSE;
    signal_setup();
    if (!line && g_interrupt_signal == TRUE)
        return ((char *)-1);
    return (line);
}

char *read_line_safe(const char *prompt)
{
    char *line;
    size_t cap;
    ssize_t len;

    if (isatty(STDIN_FILENO))
        return (read_line_tty(prompt));
    line = NULL;
    cap = 0;
    len = getline(&line, &cap, stdin);
    if (len == -1)
    {
        free_one((void **)&line);
        return (NULL);
    }
    if (len > 0 && line[len - 1] == '\n')
        line[len - 1] = '\0';
    return (line);
}
