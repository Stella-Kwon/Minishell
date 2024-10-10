/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:57:20 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 16:59:50 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_Redirection *create_redirection(void)
{
    t_Redirection *redir;

    redir = (t_Redirection *)malloc(sizeof(t_Redirection));
    if (!redir)
    {
        log_errors("Failed to malloc redirection in create_redirection", "");
        return (NULL);
    }
    redir->infile = -2;
    redir->tmp_infile = -2;
    redir->outfile = -2;
    redir->filename = NULL;
    redir->direction_type = -1;
    redir->heredoc_limiter = NULL;
    redir->herestring_str = NULL;
    return (redir);
}

int initialize_astnode(t_ASTNode **node, char ***tokens)
{
    if (node && (*node))
    {
        (*node)->type = 0;
        (*node)->command = NULL;
        (*node)->pipeline = NULL;
        (*node)->redir = NULL;
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
    if (tokens && *tokens && **tokens && is_redirection(*tokens))
    {
        (*node)->redir = create_redirection();
        if (parsing_others(tokens, &(*node)->redir, TRUE) == FAIL)
        {
            free_astnode(node);
            return (FAIL);
        }
    }
    return (SUCCESS);
}

t_Pipeline *create_pipeline(void)
{
    t_Pipeline *pipeline;

    pipeline = (t_Pipeline *)malloc(sizeof(t_Pipeline));
    if (!pipeline)
    {
        log_errors("Failed to malloc pipeline in create_pipeline", "");
        return (NULL);
    }
    pipeline->fd[0] = -2;
    pipeline->fd[1] = -2;
    pipeline->pid = -2;
    pipeline->left_pid = -2;
    pipeline->right_pid = -2;
    return (pipeline);
}