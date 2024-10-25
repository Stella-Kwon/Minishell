/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:44:05 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_child(int fd, char *limiter, char **new_input, \
						t_ASTNode **node)
{
	char	*rm_limiter;

	*new_input = NULL;
	if (dup2((*node)->term_stdin, STDIN_FILENO) == -1)
		exit(log_errors("Failed to redirect stdin in heredoc_child", ""));
	if (dup2((*node)->term_stdout, STDOUT_FILENO) == -1)
		exit(log_errors("Failed to redirect stdout in heredoc_child", ""));
	rm_limiter = rm_quotes(limiter);
	if (!rm_limiter)
	{
		free_one((void **)new_input);
		exit(log_errors("Failed to rm_quotes in heredoc_child", ""));
	}
	handle_input(fd, rm_limiter, new_input, node);
	if (new_input && *new_input)
		free_one((void **)new_input);
	free_one((void **)&rm_limiter);
	if ((*node)->command)
		free_exit(&(*node)->command, SUCCESS);
	close(fd);
	exit(SUCCESS);
}

static int	parent_heredoc(t_ASTNode **node, pid_t pid)
{
	int	status;
	int	exitcode;

	if (waitpid(pid, &status, 0) == -1)
	{
		exitcode = waitpid_status(status);
		return (exitcode);
	}
	close((*node)->redir->heredoc_infile);
	(*node)->redir->heredoc_infile = open(".heredoc.tmp", O_RDONLY);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink in here_doc", ""));
	exitcode = waitpid_status(status);
	return (exitcode);
}

int	here_doc(t_ASTNode **node, char *limiter)
{
	pid_t	pid;
	char	*new_input;

	pid = fork();
	if (pid == -1)
		return (log_errors("Failed to fork in heredoc", ""));
	(*node)->redir->heredoc_infile = open(".heredoc.tmp", \
										O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_child((*node)->redir->heredoc_infile, limiter, \
					&new_input, node);
	}
	return (parent_heredoc(node, pid));
}
