/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:25:40 by suminkwon        ###   ########.fr       */
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
