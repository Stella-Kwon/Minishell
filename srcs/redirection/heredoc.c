/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/21 22:57:22 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	append_newline_and_free(char **input)
{
	char	*tmp;

	tmp = ft_strjoin(*input, "\n");
	if (!tmp)
		return (log_errors("Failed to ft_strjoin in heredoc_child", ""));
	free_one((void **)input);
	*input = tmp;
	return (SUCCESS);
}

void	handle_input(int fd, char *limiter, char **new_input)
{
	while (1)
	{
		*new_input = readline("> ");
		if (!*new_input)
		{
			free_one((void **)new_input);
			free_one((void **)&limiter);
			exit(3);
		}
		add_history(*new_input);
		if (ft_strcmp(*new_input, limiter) == 0)
		{
			free_one((void **)new_input);
			break ;
		}
		if (append_newline_and_free(new_input) == FAIL)
			exit(FAIL);
		if (write(fd, *new_input, ft_strlen(*new_input)) == -1)
			exit(log_errors("Failed to write in heredoc child", ""));
		free_one((void **)new_input);
	}
}

int	heredoc_child(int fd, char *limiter, char **new_input)
{
	char	*rm_limiter;

	*new_input = NULL;
	rm_limiter = rm_quotes(limiter);
	if (!rm_limiter)
	{
		free_one((void **)new_input);
		exit(log_errors("Failed to rm_quotes in hereodc_child", ""));
	}
	handle_input(fd, rm_limiter, new_input);
	free_one((void **)new_input);
	free_one((void **)&rm_limiter);
	close(fd);
	exit (SUCCESS);
}

int	here_doc(t_ASTNode **node, char *limiter)
{
	int		status;
	pid_t	pid;
	char	*new_input;
	int		exitcode;

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
		heredoc_child((*node)->redir->heredoc_infile, limiter, &new_input);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		exitcode = waitpid_status(status);
		printf("exit :%d\n", exitcode);
		return (exitcode);
	}
	close((*node)->redir->heredoc_infile);
	(*node)->redir->heredoc_infile = open(".heredoc.tmp", O_RDONLY);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (unlink(".heredoc.tmp") == -1)
		return (log_errors("Failed to unlink in here_doc", ""));
	exitcode = waitpid_status(status);
	if (exitcode == 3)
	{
		ft_putstr_fd("minishell: warning: unexpected end of here-document\n", 2);
		exit(0);
	}
	return (exitcode);
}