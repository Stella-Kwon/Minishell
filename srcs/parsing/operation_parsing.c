/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 21:04:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/07 21:52:41 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	operation_and(char ***tokens, t_ASTNode **left_node, char **env)
{
	t_ASTNode	*right_node;

	(*tokens)++;
	right_node = create_astnode(tokens, NULL, NULL, env);
	if (!right_node)
		return (log_errors("NULL in RIGHT NODE : '&&' operation_parsing", ""));
	right_node->type = NODE_COMMAND;
	if (!*left_node)
		return (log_errors("NULL in LEFT NODE before AND operation", ""));
	*left_node = create_astnode(NULL, *left_node, right_node, env);
	if (!*left_node)
		return (log_errors("NULL in LEFT NODE after AND operation", ""));
	(*left_node)->type = NODE_AND;
	return (SUCCESS);
}

static int	operation_or(char ***tokens, t_ASTNode **left_node, char **env)
{
	t_ASTNode	*right_node;

	(*tokens)++;
	right_node = create_astnode(tokens, NULL, NULL, env);
	if (!right_node)
		return (log_errors("NULL in RIGHT NODE : '||' operation_parsing", ""));
	right_node->type = NODE_COMMAND;
	if (!*left_node)
		return (log_errors("NULL in LEFT NODE before OR operation", ""));
	*left_node = create_astnode(NULL, *left_node, right_node, env);
	if (!*left_node)
		return (log_errors("NULL in LEFT NODE after OR operation", ""));
	(*left_node)->type = NODE_OR;
	return (SUCCESS);
}

static int	operation_and_or(char ***tokens, t_ASTNode **left_node, char **env)
{
	if (ft_strcmp(**tokens, "&&") == 0)
		return (operation_and(tokens, left_node, env));
	else if (ft_strcmp(**tokens, "||") == 0)
		return (operation_or(tokens, left_node, env));
	return (SUCCESS);
}

int	operation_parsing(char ***tokens, t_ASTNode **left_node, char **env)
{
	t_ASTNode	*right_node;

	if (ft_strcmp(**tokens, "&&") == 0 || ft_strcmp(**tokens, "||") == 0)
	{
		if (operation_and_or(tokens, left_node, env) == FAIL)
			return (FAIL);
	}
	else if (ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		right_node = create_astnode(tokens, NULL, NULL, env);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '|' operation_parsing", \
			""));
		right_node->type = NODE_COMMAND;
		*left_node = create_astnode(tokens, *left_node, right_node, env);
		if (!*left_node)
			return (log_errors("NULL in LEFT NODE before \
			'|' in operation parsing", ""));
		(*left_node)->type = NODE_PIPE;
	}
	else
		(*tokens)++;
	return (SUCCESS);
}
