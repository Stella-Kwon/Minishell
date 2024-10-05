/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 18:56:40 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_command(t_Command **res)
{
	if (*res)
	{
		if ((*res)->cmd)
			free_one((void **)&(*res)->cmd);
		if ((*res)->args)
			all_free(&(*res)->args);
		free_one((void **)&(*res));
	}
}

void	free_redirection(t_Redirection **redir)
{
	if (!(*redir))
		return ;
	if ((*redir)->filename)
		free_one((void **)&(*redir)->filename);
	if ((*redir)->heredoc_limiter)
		free_one((void **)&(*redir)->heredoc_limiter);
	if ((*redir)->herestring_str)
		free_one((void **)&(*redir)->herestring_str);
	free_one((void **)&(*redir));
}

void	free_astnode(t_ASTNode **node)
{
	if (*node)
	{
		if ((*node)->command)
			free_command(&(*node)->command);
		if ((*node)->pipeline)
			free_one((void **)&(*node)->pipeline);
		if ((*node)->redir)
			free_redirection(&(*node)->redir);
		if ((*node)->right)
			free_astnode(&(*node)->right);
		if ((*node)->left)
			free_astnode(&(*node)->left);
		free_one((void **)&(*node));
	}
}

int	is_operator(char **tokens)
{
	if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 || \
		ft_strcmp(*tokens, "||") == 0 || ft_strcmp(*tokens, "|") == 0 || \
		ft_strcmp(*tokens, "(") == 0)
		return (1);
	return (0);
}

int	get_direction_type(char *token)
{
	if (ft_strcmp(token, ">>") == 0)
		return (REDIRECT_APPEND);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUTPUT);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_INPUT);
	return (INVALID);
}

int	handle_redirection(char **args)
{
	int	i;
	int	redirection_found;
	int	fd_in;
	int	fd_out;

	i = 0;
	redirection_found = 0;
	fd_in = -1;
	fd_out = -1;

	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 && !redirection_found)
		{
			fd_in = open(args[i + 1], O_RDONLY);
			if (fd_in < 0)
			{
				log_errors("Opening file error: input redirection", "");
				return (FAIL);
			}
			redirection_found = 1;
		}
		else if (ft_strcmp(args[i], ">") == 0 && !redirection_found)
		{
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out < 0)
			{
				log_errors("Opening file error: output redirection", "");
				return (FAIL);
			}
			redirection_found = 1;
		}
		i++;
	}
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != -1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	return (SUCCESS);
}