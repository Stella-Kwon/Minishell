/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 17:48:06 by sukwon           ###   ########.fr       */
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
			line = ft_strjoin(line, "\n");
			if (!line)
				return (log_errors("Failed to ft_strjoin in heredoc_child", \
				""));
			else
				free_one((void **)&line);
			close(fd);
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

int	here_doc(t_ASTNode **node)
{
	(*node)->redir->infile = open(".heredoc.tmp", \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*node)->redir->infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	(*node)->command->exitcode = heredoc_child((*node)->redir->infile, \
			(*node)->redir->heredoc_limiter);
	close((*node)->redir->infile);
	(*node)->redir->infile = open(".heredoc.tmp", O_RDONLY);
	if ((*node)->redir->infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink in here_doc", ""));
	return ((*node)->command->exitcode);
}
