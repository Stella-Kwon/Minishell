/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 19:04:24 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_child(int fd, char *limiter)
{
    char *line;

    line = NULL;
    while (read_line(&line) != FAIL)
    {
        if (line == NULL)
            break;
        if (ft_strcmp(line, limiter) == 0)
        {
            free_one((void **)&line); // line 해제
            return (SUCCESS);         // 성공적으로 종료
        }
        line = ft_strjoin(line, "\n");
        if (write(fd, line, ft_strlen(line)) == -1)
            log_errors("Failed to write in heredoc childe", "");
        free_one((void **)&line);
        ft_putstr_fd("> ", 0);
    }
    free_one((void **)&line);
    return (FAIL);
}

int here_doc(ASTNode **node)
{
    int exitcode;

    (*node)->redir->infile = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if ((*node)->redir->infile == -1)
        return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
    ft_putstr_fd("> ", 0);
    exitcode = heredoc_child((*node)->redir->infile, (*node)->redir->heredoc_limiter);
    close((*node)->redir->infile);
    // printf("(*node)->redir->infile: %d\n", (*node)->redir->infile);
    (*node)->redir->infile = open(".heredoc.tmp", O_RDONLY);
    if ((*node)->redir->infile == -1)
        return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
    if (unlink(".heredoc.tmp") == -1) // unlink로 파일을 시스템에서 삭제 (이미 열린 FD에는 영향 없음)
        return (log_errors("Failed to unlink in here_doc", ""));
    return (exitcode);
}
