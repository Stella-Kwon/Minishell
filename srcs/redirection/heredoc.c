/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/23 22:57:32 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void terminal_input_output(int *prev_stdin, int *prev_stdout,
								  int *tty_stdin, int *tty_stdout)
{
	*tty_stdin = open("/dev/tty", O_RDONLY);
	*tty_stdout = open("/dev/tty", O_WRONLY);
	if (*tty_stdin == -1 || *tty_stdout == -1)
	{
		if (*tty_stdin != -1)
			close(*tty_stdin);
		exit(log_errors("Failed to open /dev/tty in heredoc_child", ""));
	}
	*prev_stdin = dup(STDIN_FILENO);
	if (dup2(*tty_stdin, STDIN_FILENO) == -1)
	{
		close(*tty_stdin);
		close(*tty_stdout);
		exit(log_errors("Failed to redirect stdin in heredoc_child", ""));
	}
	*prev_stdout = dup(STDOUT_FILENO);
	if (dup2(*tty_stdout, STDOUT_FILENO) == -1)
	{
		dup2(*prev_stdin, STDIN_FILENO);
		close(*prev_stdin);
		close(*tty_stdin);
		close(*tty_stdout);
		exit(log_errors("Failed to redirect stdout in heredoc_child", ""));
	}
}

static void restore_input_output(int prev_stdin, int prev_stdout,
								 int tty_stdin, int tty_stdout)
{
	dup2(prev_stdin, STDIN_FILENO);
	dup2(prev_stdout, STDOUT_FILENO);
	close(prev_stdin);
	close(prev_stdout);
	close(tty_stdin);
	close(tty_stdout);
}

static int heredoc_child(int fd, char *limiter, char **new_input, t_ASTNode **node)
{
	char *rm_limiter;
	int tty_stdin;
	int tty_stdout;
	int prev_stdin;
	int prev_stdout;

	*new_input = NULL;
	terminal_input_output(&prev_stdin, &prev_stdout, &tty_stdin, &tty_stdout);
	rm_limiter = rm_quotes(limiter);
	if (!rm_limiter)
	{
		free_one((void **)new_input);
		restore_input_output(prev_stdin, prev_stdout, tty_stdin, tty_stdout);
		exit(log_errors("Failed to rm_quotes in heredoc_child", ""));
	}
	handle_input(fd, rm_limiter, new_input, node);
	restore_input_output(prev_stdin, prev_stdout, tty_stdin, tty_stdout);
	if (new_input && *new_input)
		free_one((void **)new_input);
	free_one((void **)&rm_limiter);
	if ((*node)->command)
		free_exit(&(*node)->command, SUCCESS);
	close(fd);
	exit(SUCCESS);
}

static int parent_heredoc(t_ASTNode **node, pid_t pid)
{
	int status;
	int exitcode;

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

int here_doc(t_ASTNode **node, char *limiter)
{
	pid_t pid;
	char *new_input;

	pid = fork();
	if (pid == -1)
		return (log_errors("Failed to fork in heredoc", ""));
	(*node)->redir->heredoc_infile = open(".heredoc.tmp",
										  O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_child((*node)->redir->heredoc_infile, limiter, &new_input, node);
	}
	return (parent_heredoc(node, pid));
}