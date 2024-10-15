/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/06 21:11:09 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_child(int fd, char *limiter)
{
	char	*line;

	line = NULL;
	while (read_line(&line) != FAIL)
	{
		if (ft_strcmp(line, limiter) == 0)
		{
			free_one((void **)&line);
			return (SUCCESS);
		}
		line = ft_strjoin(line, "\n");
		if (write(fd, line, ft_strlen(line)) == -1)
			log_errors("Failed to write in heredoc child", "");
		free_one((void **)&line);
		ft_putstr_fd("> ", 0);
	}
	return (FAIL);
}

int	here_doc(t_ASTNode **node, char *limiter)
{
	int	exitcode;

	(*node)->redir->heredoc_infile = open(".heredoc.tmp", \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	ft_putstr_fd("> ", 0);
	exitcode = heredoc_child((*node)->redir->heredoc_infile, limiter);
	close((*node)->redir->heredoc_infile);
	(*node)->redir->heredoc_infile = open(".heredoc.tmp", O_RDONLY);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink in here_doc", ""));
	return (exitcode);
}
