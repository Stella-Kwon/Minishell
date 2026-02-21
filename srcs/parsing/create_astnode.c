/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_astnode.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 18:42:27 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int create_astnode_content(t_ASTNode *ast, char ***tokens, char ***env, t_TokenizeResult *result)
{
	if (tokens && *tokens && **tokens)
	{
		if (is_operator(**tokens) == 1)
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
	if (ast->command &&
		(parsing_others(&ast->command->args, ast->redir, FALSE, result) == FAIL))
	{
		free_astnode(&ast);
		return (FAIL);
	}
	return (SUCCESS);
}

t_ASTNode *create_astnode(char ***tokens, t_ASTNode *left,
						  t_ASTNode *right, char ***env, t_TokenizeResult *result)
{
	t_ASTNode *ast;

	ast = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	if (!ast)
	{
		log_errors("Failed to malloc node in create_astnode", "");
		return (NULL);
	}
	if (initialize_astnode(&ast, tokens, result) == FAIL ||
		create_astnode_content(ast, tokens, env, result) == FAIL)
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

t_ASTNode *parse_pipe_operations(char ***tokens, char ***env, t_TokenizeResult *result)
{
	t_ASTNode *left_node;
	t_ASTNode *right_node;

	if (!tokens || !*tokens || !**tokens)
		return (NULL);
	left_node = create_astnode(tokens, NULL, NULL, env, result);
	if (!left_node)
		return (NULL);
	left_node->type = NODE_COMMAND;
	while (*tokens && **tokens && ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		right_node = create_astnode(tokens, NULL, NULL, env, result);
		if (!right_node)
			return (NULL);
		right_node->type = NODE_COMMAND;
		left_node = create_astnode(NULL, left_node, right_node, env, result);
		left_node->command = create_pipe_command(env);
		left_node->type = NODE_PIPE;
	}
	return (left_node);
}

int and_or_operation_parsing(char ***tokens, t_ASTNode **left_node, char ***env, t_TokenizeResult *result)
{
	t_ASTNode *right_node;
	int node_type;

	if (!tokens || !*tokens || !**tokens)
		return (FAIL);
	if (ft_strcmp(**tokens, "&&") == 0 || ft_strcmp(**tokens, "||") == 0)
	{
		if (ft_strcmp(**tokens, "&&") == 0)
			node_type = NODE_AND;
		else if (ft_strcmp(**tokens, "||") == 0)
			node_type = NODE_OR;
		(*tokens)++;
		right_node = parse_pipe_operations(tokens, env, result);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE", ""));
		*left_node = create_astnode(NULL, *left_node, right_node, env, result);
		(*left_node)->type = node_type;
	}
	return (SUCCESS);
}

t_ASTNode *parse_to_nodes(t_TokenizeResult *result, char ***env)
{
	t_ASTNode *left_node;
	char **tokens;

	if (!result || !result->tokens || !*(result->tokens))
		return (NULL);
	tokens = result->tokens;
	left_node = parse_pipe_operations(&tokens, env, result);
	if (!left_node)
		return (NULL);
	while (*tokens)
	{
		if (and_or_operation_parsing(&tokens, &left_node, env, result) == FAIL)
			return (NULL);
	}
	return (left_node);
}
