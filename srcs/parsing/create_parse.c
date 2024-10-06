/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/06 21:32:35 by suminkwon        ###   ########.fr       */
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
		return (NULL);
	if (ast->command && ast->command->args)
		remove_args_after_redirection(&ast->command->args);
	ast->left = left;
	ast->right = right;
	return (ast);
}

static int	operation_and_or(char ***tokens, t_ASTNode **left_node, char **env)
{
	t_ASTNode	*right_node;

	if (ft_strcmp(**tokens, "&&") == 0)
	{
		(*tokens)++;
		right_node = create_astnode(tokens, NULL, NULL, env);
		right_node->type = NODE_COMMAND;
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '&&' operation_parsing", \
					""));
		*left_node = create_astnode(NULL, *left_node, right_node, env);
		(*left_node)->type = NODE_AND;
	}
	else if (ft_strcmp(**tokens, "||") == 0)
	{
		(*tokens)++;
		right_node = create_astnode(tokens, NULL, NULL, env);
		right_node->type = NODE_COMMAND;
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '||' operation_parsing", \
					""));
		*left_node = create_astnode(NULL, *left_node, right_node, env);
		(*left_node)->type = NODE_OR;
	}
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
		right_node->type = NODE_COMMAND;
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '|' operation_parsing", \
					""));
		*left_node = create_astnode(tokens, *left_node, right_node, env);
		(*left_node)->type = NODE_PIPE;
	}
	else
		(*tokens)++;
	return (SUCCESS);
}

t_ASTNode	*parse_to_nodes(char **tokens, char **env)
{
	t_ASTNode	*left_node;

	if (!tokens || !*tokens)
		return (NULL);
	if (*tokens)
	{
		left_node = create_astnode(&tokens, NULL, NULL, env);
		left_node->type = NODE_COMMAND;
		if (!left_node)
			return (NULL);
	}
	while (*tokens)
	{
		if (operation_parsing(&tokens, &left_node, env) == FAIL)
			return (NULL);
	}
	return (left_node);
}
