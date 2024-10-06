/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/06 19:17:18 by suminkwon        ###   ########.fr       */
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
