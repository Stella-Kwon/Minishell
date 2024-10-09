/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/07 21:03:57 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	create_astnode_content(t_ASTNode *ast, char ***tokens, char **env)
{
	if (tokens && *tokens && **tokens)
	{
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
	if (ast->command)
	{
		if (parsing_others(&ast->command->args, &ast->redir, FALSE) == FAIL)
		{
			free_astnode(&ast);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

t_ASTNode	*create_astnode(char ***tokens, t_ASTNode *left, \
							t_ASTNode *right, char **env)
{
	t_ASTNode	*ast;

	ast = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	if (!ast)
	{
		log_errors("Failed to malloc node in create_astnode", "");
		return (NULL);
	}
	if (initialize_astnode(&ast, tokens) == FAIL)
	{
		free(ast);
		return (NULL);
	}
	if (create_astnode_content(ast, tokens, env) == FAIL)
	{
		free_astnode(&ast);
		return (NULL);
	}
	if (ast->command && ast->command->args)
		remove_args_after_redirection(&ast->command->args);
	ast->left = left;
	ast->right = right;
	return (ast);
}

t_ASTNode	*parse_to_nodes(char **tokens, char **env)
{
	t_ASTNode	*left_node;

	if (!tokens || !*tokens)
		return (NULL);
	if (*tokens)
	{
		left_node = create_astnode(&tokens, NULL, NULL, env);
		if (!left_node)
			return (NULL);
		left_node->type = NODE_COMMAND;
	}
	while (*tokens)
	{
		if (operation_parsing(&tokens, &left_node, env) == FAIL)
			return (NULL);
	}
	return (left_node);
}
