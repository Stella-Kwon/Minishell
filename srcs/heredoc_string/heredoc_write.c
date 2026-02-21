/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:44:05 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int here_doc_write(t_Redirection *redir, int heredoc_index)
{
	int fd;
	char *body;

	fd = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (log_errors("Failed to open .heredoc.tmp", ""));
	if (redir->heredoc_body && redir->heredoc_body[heredoc_index])
	{
		body = redir->heredoc_body[heredoc_index];
		if (write(fd, body, ft_strlen(body)) == -1)
		{
			close(fd);
			return (log_errors("Failed to write heredoc body", ""));
		}
	}
	close(fd);
	if (redir->infile >= 0)
		close(redir->infile);
	redir->infile = open(".heredoc.tmp", O_RDONLY);
	if (redir->infile == -1)
		return (log_errors("Failed to reopen .heredoc.tmp", ""));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink .heredoc.tmp", ""));
	return (SUCCESS);
}

static int iterate_heredoc(t_ASTNode **node, int i)
{
	int exitcode;

	if ((*node)->command)
	{
		(*node)->command->exitcode = here_doc_write((*node)->redir, i);
		exitcode = (*node)->command->exitcode;
	}
	else
	{
		exitcode = here_doc_write((*node)->redir, i);
	}
	return (exitcode);
}

int heredoc_check(t_ASTNode **node)
{
	int i;
	int exit;

	exit = 0;
	i = 0;
	if ((*node)->redir && (*node)->redir->heredoc_limiter && (*node)->redir->heredoc_limiter[0])
	{
		// if ((*node)->redir->last_stdin_type != 2)
		// 	return (SUCCESS);
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

int prepare_heredoc_herestring(t_ASTNode **node, char **env, int last_exit_code)
{
	int exitcode;

	if (!node || !*node)
		return (SUCCESS);
	if ((*node)->redir)
	{
		if ((*node)->redir->heredoc_limiter)
		{
			exitcode = heredoc_check(node);
			if (exitcode != SUCCESS)
				return (exitcode);
		}
		if ((*node)->redir->herestring_str)
		{
			exitcode = process_herestring(node, env, last_exit_code);
			if (exitcode != SUCCESS)
				return (exitcode);
		}
	}
	if ((*node)->left)
	{
		exitcode = prepare_heredoc_herestring(&(*node)->left, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	if ((*node)->right)
	{
		exitcode = prepare_heredoc_herestring(&(*node)->right, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	return (SUCCESS);
}
