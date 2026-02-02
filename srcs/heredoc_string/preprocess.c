/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/24 21:46:22 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_herestring_parsing(char ***args, t_Redirection **redirect)
{
	if (ft_strcmp(**args, "<<") == 0)
	{
		(*args)++;
		if (set_heredoc(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else if (ft_strcmp(**args, "<<<") == 0)
	{
		if (herestring_action(args, redirect) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

int heredoc_preprocess(t_ASTNode **node, char **env, int last_exit_code)
{
	int exitcode;

	if (!node || !*node)
		return (SUCCESS);
	if ((*node)->redir && (*node)->redir->heredoc_limiter)
	{
		exitcode = read_all_heredocs(node, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	if ((*node)->redir && (*node)->redir->herestring_str)
	{
		exitcode = process_herestring(node, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	if ((*node)->left)
	{
		exitcode = heredoc_preprocess(&(*node)->left, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	if ((*node)->right)
	{
		exitcode = heredoc_preprocess(&(*node)->right, env, last_exit_code);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	if ((*node)->redir && (*node)->redir->heredoc_limiter)
	{
		exitcode = heredoc_check(node);
		if (exitcode != SUCCESS)
			return (exitcode);
	}
	return (SUCCESS);
}
