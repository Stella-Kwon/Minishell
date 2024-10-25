/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:57:20 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/24 22:43:11 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_Redirection	*create_redirection(void)
{
	t_Redirection	*redir;

	redir = (t_Redirection *)ft_calloc(1, sizeof(t_Redirection));
	if (!redir)
	{
		log_errors("Failed to ft_calloc redirection in create_redirection", "");
		return (NULL);
	}
	redir->infile = -2;
	redir->heredoc_infile = -2;
	redir->outfile = -2;
	redir->in_filename = NULL;
	redir->out_filename = NULL;
	redir->direction_type = -1;
	redir->heredoc_limiter = NULL;
	redir->heredoc_i = 0;
	redir->herestring_str = NULL;
	redir->heredoc_buffsize = BUFFER_SIZE;
	return (redir);
}

int	initialize_astnode(t_ASTNode **node, char ***tokens)
{
	if (node && (*node))
	{
		(*node)->type = 0;
		(*node)->command = NULL;
		(*node)->pipeline = NULL;
		(*node)->redir = NULL;
		(*node)->left = NULL;
		(*node)->right = NULL;
		(*node)->term_stdin = dup(STDIN_FILENO);
		(*node)->term_stdout = dup(STDOUT_FILENO);
	}
	if (tokens && *tokens && **tokens && is_redirection(**tokens))
	{
		(*node)->redir = create_redirection();
		if (!(*node)->redir)
			return (FAIL);
		if (parsing_others(tokens, &(*node)->redir, TRUE) == FAIL)
		{
			free_astnode(node);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

t_Pipeline	*create_pipeline(void)
{
	t_Pipeline	*pipeline;

	pipeline = (t_Pipeline *)ft_calloc(1, sizeof(t_Pipeline));
	if (!pipeline)
	{
		log_errors("Failed to ft_calloc pipeline in create_pipeline", "");
		return (NULL);
	}
	pipeline->fd[0] = -2;
	pipeline->fd[1] = -2;
	pipeline->pid = -2;
	pipeline->left_pid = -2;
	pipeline->right_pid = -2;
	return (pipeline);
}
