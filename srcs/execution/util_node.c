/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 23:24:53 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_check(t_ASTNode	**node)
{
	if ((*node)->redir->heredoc_limiter)
	{
		if (here_doc(node) == FAIL)
			return (log_errors("Failed here_doc in heredoc_check", ""));
		if ((*node)->redir->tmp_infile != -2)
			(*node)->redir->infile = (*node)->redir->tmp_infile;
	}
	return (SUCCESS);
}

void	set_last_exitcode(t_ASTNode	**node, int last_exitcode)
{
	(*node)->last_exitcode = last_exitcode;
	if ((*node)->left)
		set_last_exitcode(&(*node)->left, last_exitcode);
	if ((*node)->right)
		set_last_exitcode(&(*node)->right, last_exitcode);
}

static void	read_exitcode(t_ASTNode **node, int *exitcode)
{
	if (*node)
	{
		if ((*node)->command && (*node)->command->exitcode != -1)
		{
			*exitcode = (*node)->command->exitcode;
		}
		if ((*node)->right)
			read_exitcode(&(*node)->right, exitcode);
		if ((*node)->left)
			read_exitcode(&(*node)->left, exitcode);
	}
}

void	get_last_exitcode(t_ASTNode	**node, int *last_exitcode)
{
	int exitcode;

	exitcode = -1;
	read_exitcode(node, &exitcode);
	if (exitcode != -1)
		*last_exitcode = exitcode;
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
