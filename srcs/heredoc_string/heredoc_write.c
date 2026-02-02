/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:44:05 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	here_doc_write(t_Redirection *redir, int heredoc_index)
{
	int	fd;
	int	i;

	fd = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (log_errors("Failed to open .heredoc.tmp", ""));
	i = 0;
	while (redir->heredoc_body[heredoc_index] &&
		redir->heredoc_body[heredoc_index][i])
	{
		write(fd, redir->heredoc_body[heredoc_index][i],
			ft_strlen(redir->heredoc_body[heredoc_index][i]));
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
	redir->heredoc_infile = open(".heredoc.tmp", O_RDONLY);
	if (redir->heredoc_infile == -1)
		return (log_errors("Failed to reopen .heredoc.tmp", ""));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink .heredoc.tmp", ""));
	return (SUCCESS);
}

static int	iterate_heredoc(t_ASTNode **node, int i)
{
	int	exitcode;

	if ((*node)->command)
	{
		(*node)->command->exitcode = here_doc_write((*node)->redir, i);
		exitcode = (*node)->command->exitcode;
	}
	else
	{
		exitcode = here_doc_write((*node)->redir, i);
	}
	if (exitcode != SUCCESS)
		return (exitcode);
	if ((*node)->redir->last_stdin_type == 2)
	{
		if ((*node)->redir->heredoc_infile != -2 && (*node)->redir->infile != -1)
		{
			if ((*node)->redir->infile >= 0)
				close((*node)->redir->infile);
			(*node)->redir->infile = (*node)->redir->heredoc_infile;
		}
	}
	return (SUCCESS);
}

int	heredoc_check(t_ASTNode **node)
{
	int	i;
	int	exit;

	exit = 0;
	i = 0;
	if ((*node)->redir->heredoc_limiter && (*node)->redir->heredoc_limiter[0])
	{
		while ((*node)->redir->heredoc_limiter[i] &&
			i < (*node)->redir->heredoc_i)
		{
			exit = iterate_heredoc(node, i);
			if (exit != SUCCESS)
				return (exit);
			i++;
		}
	}
	return (SUCCESS);
}
