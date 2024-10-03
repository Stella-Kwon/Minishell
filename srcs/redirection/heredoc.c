/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:58:15 by suminkwon        ###   ########.fr       */
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
                return (log_errors("Failed to ft_strjoin in heredoc_child", ""));
            else
                free_one((void **)&line);
            close(fd);
            return (SUCCESS);
        }
        line = ft_strjoin(line, "\n");
        if (write(fd, line, ft_strlen(line)) == -1)
            log_errors("Failed to write in heredoc childe", "");
        free_one((void **)&line);
        ft_putstr_fd("> ", 0);
    }
    return (FAIL);
}

int here_doc(ASTNode **node)
{
    (*node)->redir->infile = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if ((*node)->redir->infile == -1)
        return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
    (*node)->command->exitcode = heredoc_child((*node)->redir->infile, (*node)->redir->heredoc_limiter);
    close((*node)->redir->infile);
    (*node)->redir->infile = open(".heredoc.tmp", O_RDONLY);
    if ((*node)->redir->infile == -1)
        return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
    if (unlink(".heredoc.tmp") == -1) // unlink로 파일을 시스템에서 삭제 (이미 열린 FD에는 영향 없음)
        return (log_errors("Failed to unlink in here_doc", ""));
    return ((*node)->command->exitcode);
}
