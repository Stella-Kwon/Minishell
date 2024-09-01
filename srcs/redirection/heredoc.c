/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 14:03:34 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int open_file(char *file, int fileno, int heredoc)
{
    int open_fd;

    open_fd = 0;
    if (fileno == 0 && heredoc == 0) // infile
        open_fd = open(file, O_RDONLY);
    else if (fileno == 1 && heredoc == 1)//heredoc && >> append 용
        open_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (fileno == 1 && heredoc == 0)//outfile
        open_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (open_fd == -1)
    {
        // ft_putstr_fd("bash: ", 2);
        // errors(file, 0, NULL, NULL);
        // return (-1);
        return (log_errors("open_file is failed"));
    }
    return (open_fd);
}

int ft_strcmp(const char *line, const char *limiter)
{
    size_t i;
    size_t n;

    i = 0;
    n = ft_strlen(limiter);
    if ((ft_strlen(line)) != n)
        return (-1);
    while (i < n && (line[i] != '\0' || limiter[i] != '\0'))
    {
        if ((unsigned char)limiter[i] != (unsigned char)line[i])
            return (-2);
        i++;
    }
    return (0);
}

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
                return (log_errors("heredoc : adding newline in ft_strjoin failed"));
            else
                free(line);
            close(fd);
            return (SUCCESS);
        }
        line = ft_strjoin(line, "\n");
        if (!line)
            return (log_errors("heredoc : adding newline in ft_strjoin failed"));
        if (write(fd, line, ft_strlen(line)) == -1)
            return ((log_errors("heredoc : write failed")));
        free(line);
        line = NULL;
        ft_putstr_fd("> ", 0);
    }
    return (FAIL);
}

int here_doc(Command **node)
{
    (*node)->infile = open_file(".tmp", 1, 1);
    if ((*node)->infile == -1)
        return (FAIL);
    ft_putstr_fd("> ", 0);
    (*node)->exitcode = heredoc_child((*node)->infile, (*node)->heredoc_limiter);
    close((*node)->infile);
    // (*node)->infile = open_file(".tmp", 0, 0);
    // if ((*node)->infile == -1)
    //     return (FAIL);
    return ((*node)->exitcode);
}
