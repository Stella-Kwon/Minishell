/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:36:33 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/22 21:50:55 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int append_newline_and_free(char **input)
{
	char *tmp;

	tmp = ft_strjoin(*input, "\n");
	if (!tmp)
		return (log_errors("Failed to ft_strjoin in heredoc_child", ""));
	free_one((void **)input);
	*input = tmp;
	return (SUCCESS);
}

void	check_empty_input_and_dollar_sign(char **new_input, t_ASTNode **node, char **copy_new)
{
	if (!*new_input)
	{
		free_one((void **)new_input);
		exit(3);
	}
	*copy_new = ft_strdup(*new_input);
	if ((*node)->command)
	{
		if (find_dollar_signs(new_input, *((*node)->command->env), (*node)->last_exitcode) == FAIL)
			log_errors("Command expansion failed", *new_input);
	}
}

void handle_input(int fd, char *limiter, char **new_input, t_ASTNode **node)
{
	char *copy_new;

	while (1)
	{
		*new_input = readline("> ");
		if (!(*new_input))
		{
			free_one((void **)&copy_new);
			free_one((void **)new_input);
			free_one((void **)&limiter);
			free_exit(&(*node)->command, SUCCESS);
		}
		check_empty_input_and_dollar_sign(new_input, node, &copy_new);

		if (ft_strcmp(copy_new, limiter) == 0)
		{
			free_one((void **)&copy_new);
			break;
		}
		if (append_newline_and_free(new_input) == FAIL)
		{
			free_one((void **)&copy_new);
			free_one((void **)new_input);
			free_one((void **)&limiter);
			exit(FAIL);
		}
		if (write(fd, *new_input, ft_strlen(*new_input)) == -1)
		{
			log_errors("Failed to write in heredoc child", "");
			free_one((void **)&copy_new);
			free_one((void **)new_input);
			free_one((void **)&limiter);
			free_exit(&(*node)->command, SUCCESS);
		}
		free_one((void **)new_input);
		free_one((void **)&copy_new);
	}
}

int	heredoc_child(int fd, char *limiter, char **new_input, t_ASTNode **node)
{
	char *rm_limiter;

	*new_input = NULL;
	rm_limiter = rm_quotes(limiter);
	if (!rm_limiter)
	{
		free_one((void **)new_input);
		exit(log_errors("Failed to rm_quotes in hereodc_child", ""));
	}
	handle_input(fd, rm_limiter, new_input, node);
	free_one((void **)new_input);
	free_one((void **)&rm_limiter);
	close(fd);
	free_exit(&(*node)->command, SUCCESS);
	exit(SUCCESS);
}

int	parent_heredoc(t_ASTNode **node, pid_t pid)
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
	(*node)->redir->heredoc_infile = open(".heredoc.tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*node)->redir->heredoc_infile == -1)
		return (log_errors(".heredoc.tmp", "Failed to open file in here_doc"));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		heredoc_child((*node)->redir->heredoc_infile, limiter, &new_input, node);
	}
	return (parent_heredoc(node, pid));
}
