/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 17:27:39 by hlee-sun         ###   ########.fr       */
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

void	set_last_exitcode(t_ASTNode	**node, int last_exit_code)
{
	(*node)->last_exit_code = last_exit_code;
	if ((*node)->left)
		set_last_exitcode(&(*node)->left, last_exit_code);
	if ((*node)->right)
		set_last_exitcode(&(*node)->right, last_exit_code);
}

int dup_and_close(int oldfd, int newfd) 
{
    if (dup2(oldfd, newfd) == -1) {
        close(oldfd);
        return (log_errors("Failed to dup2 in action_child", ""));
    }
    close(oldfd);
    return (SUCCESS);
}
