/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 15:45:41 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	iterate_heredoc(t_ASTNode **node, int i)
{
	int	exitcode;

	if ((*node)->command)
	{
		(*node)->command->exitcode = here_doc(node,
								(*node)->redir->heredoc_limiter[i]);
		exitcode = (*node)->command->exitcode;
	}
	else
	{
		exitcode = here_doc(node, (*node)->redir->heredoc_limiter[i]);
	}
	if (exitcode != SUCCESS)
		return (exitcode);
	if ((*node)->redir->heredoc_infile != -2 && (*node)->redir->infile != -1)
	{
		if ((*node)->redir->infile >= 0)
			close((*node)->redir->infile);
		(*node)->redir->infile = (*node)->redir->heredoc_infile;
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
		while ((*node)->redir->heredoc_limiter[i] && \
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

int	dup_and_close(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		close(oldfd);
		return (log_errors("Failed to dup2 in action_child", ""));
	}
	close(oldfd);
	return (SUCCESS);
}
