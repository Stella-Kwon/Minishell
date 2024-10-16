/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:55:15 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 11:09:56 by sukwon           ###   ########.fr       */
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
	if ((*redir)->in_filename)
		free_one((void **)&(*redir)->in_filename);
	if ((*redir)->out_filename)
		free_one((void **)&(*redir)->out_filename);
	if ((*redir)->heredoc_limiter)
		all_free(&(*redir)->heredoc_limiter);
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
