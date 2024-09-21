/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/21 16:55:58 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_child(int fd, char *limiter)
{
    char *line;

    line = NULL;
    while (read_line(&line) != FAIL)
    {
        if (ft_strcmp(line, limiter) == 0)
        {
            line = ft_strjoin(line, "\n");
            if (!line)
                return (errors("read-line ft_strjoin error", 0, NULL, NULL));
            else
                free_one(line);
            close(fd);
            return (SUCCESS);
        }
        line = ft_strjoin(line, "\n");
        if (write(fd, line, ft_strlen(line)) == -1)
            errors("write", 0, NULL, NULL);
        free_one(line);
        ft_putstr_fd("> ", 0);
    }
}

int here_doc(ASTNode **node)
{
    (*node)->command->infile = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if ((*node)->command->infile == -1)
        return (log_errors("Failed to open file in here_doc"));
    (*node)->command->exitcode = heredoc_child((*node)->command->infile, (*node)->command->heredoc_limiter);
    close((*node)->command->infile);
    (*node)->command->infile = open(".heredoc.tmp", O_RDONLY);
    if ((*node)->command->infile == -1)
        return (log_errors("Failed to open file in here_doc"));
    if (unlink(".heredoc.tmp") == -1)// unlink로 파일을 시스템에서 삭제 (이미 열린 FD에는 영향 없음)
        return (log_errors("Failed to unlink in here_doc"));
    return ((*node)->command->exitcode);
}
