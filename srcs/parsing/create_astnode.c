/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_astnode.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 00:42:22 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_astnode_content(t_ASTNode *ast, char ***tokens, char ***env)
{
	if (tokens && *tokens && **tokens)
	{
		if (is_operator(**tokens))
			return (SUCCESS);
		ast->command = create_command(tokens, env);
		if (!ast->command)
			return (FAIL);
	}
	ast->pipeline = create_pipeline();
	if (!ast->pipeline)
		return (FAIL);
	if (ast->redir == NULL)
	{
		ast->redir = create_redirection();
		if (!ast->redir)
			return (FAIL);
	}
	if (ast->command && \
		(parsing_others(&ast->command->args, &ast->redir, FALSE) == FAIL))
	{
		free_astnode(&ast);
		return (FAIL);
	}
	return (SUCCESS);
}

t_ASTNode	*create_astnode(char ***tokens, t_ASTNode *left, \
							t_ASTNode *right, char ***env)
{
	t_ASTNode	*ast;

	ast = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	if (!ast)
	{
		log_errors("Failed to malloc node in create_astnode", "");
		return (NULL);
	}
	if (initialize_astnode(&ast, tokens) == FAIL || \
		create_astnode_content(ast, tokens, env) == FAIL)
	{
		free(ast);
		return (NULL);
	}
	if (ast->command && ast->command->args)
	{
		if (remove_args_after_redirection(&ast->command->args) != SUCCESS)
		{
			free_astnode(&ast);
			return (NULL);
		}
	}
	ast->left = left;
	ast->right = right;
	return (ast);
}

t_ASTNode	*parse_pipe_operations(char ***tokens, char ***env)
{
	t_ASTNode	*left_node;
	t_ASTNode	*right_node;

	if (!tokens || !*tokens || !**tokens)
		return (NULL);
	left_node = create_astnode(tokens, NULL, NULL, env);
	if (!left_node)
		return (NULL);
	left_node->type = NODE_COMMAND;
	while (*tokens && **tokens && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		right_node = create_astnode(tokens, NULL, NULL, env);
		if (!right_node)
			return (NULL);
		right_node->type = NODE_COMMAND;
		left_node = create_astnode(NULL, left_node, right_node, env);
		left_node->command = create_pipe_command(env);
		left_node->type = NODE_PIPE;
	}
	return (left_node);
}

int	and_or_operation_parsing(char ***tokens, t_ASTNode **left_node, char ***env)
{
	t_ASTNode	*right_node;
	int			node_type;

	if (!tokens || !*tokens || !**tokens)
		return (FAIL);
	if (ft_strcmp(**tokens, "&&") == 0 || ft_strcmp(**tokens, "||") == 0)
	{
		if (ft_strcmp(**tokens, "&&") == 0)
			node_type = NODE_AND;
		else if (ft_strcmp(**tokens, "||") == 0)
			node_type = NODE_OR;
		(*tokens)++;
		right_node = parse_pipe_operations(tokens, env);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE", ""));
		*left_node = create_astnode(NULL, *left_node, right_node, env);
		(*left_node)->type = node_type;
	}
	return (SUCCESS);
}

t_ASTNode	*parse_to_nodes(char **tokens, char ***env)
{
	t_ASTNode	*left_node;

	if (!tokens || !*tokens)
		return (NULL);
	left_node = parse_pipe_operations(&tokens, env);
	if (!left_node)
		return (NULL);
	while (*tokens)
	{
		if (and_or_operation_parsing(&tokens, &left_node, env) == FAIL)
			return (NULL);
	}
	return (left_node);
}
