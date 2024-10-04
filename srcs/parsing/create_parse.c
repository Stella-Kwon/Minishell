/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 01:53:35 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirection(t_Redirection **redir)
{
	if (!(*redir))
		return ;
	if ((*redir)->filename)
		free_one((void **)&(*redir)->filename);
	if ((*redir)->heredoc_limiter)
		free_one((void **)&(*redir)->heredoc_limiter);
	if ((*redir)->herestring_str)
		free_one((void **)&(*redir)->herestring_str);
	if ((*redir)->dollar_vari)
		free_one((void **)&(*redir)->dollar_vari);
	free_one((void **)&(*redir));
}

int	is_redirection(char **token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (ft_strcmp(token[i], "<") == 0 || ft_strcmp(token[i], "<<") == 0 || \
		ft_strcmp(token[i], "<<<") == 0 || ft_strcmp(token[i], ">") == 0 || \
		ft_strcmp(token[i], ">>") == 0)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

t_Redirection	*create_redirection(void)
{
	t_Redirection	*redir;

	redir = (t_Redirection *)malloc(sizeof(t_Redirection));
	if (!redir)
	{
		log_errors("Failed to malloc redirection in create_redirection", "");
		return (NULL);
	}
	redir->infile = -2;
	redir->outfile = -2;
	redir->filename = NULL;
	redir->direction_type = -1;
	redir->heredoc_limiter = NULL;
	redir->herestring_str = NULL;
	redir->dollar_vari = NULL;
	return (redir);
}

void	free_ASTNODE(t_ASTNode **node)
{
	if (*node)
	{
		if ((*node)->command)
			free_Command(&(*node)->command);
		if ((*node)->pipeline)
			free_one((void **)&(*node)->pipeline);
		if ((*node)->redir)
			free_redirection(&(*node)->redir);
		if ((*node)->right)
			free_ASTNODE(&(*node)->right);
		if ((*node)->left)
			free_ASTNODE(&(*node)->left);
		free_one((void **)&(*node));
	}
}

t_Pipeline	*create_pipeline(void)
{
	t_Pipeline	*pipeline;

	pipeline = (t_Pipeline *)malloc(sizeof(t_Pipeline));
	if (!pipeline)
	{
		log_errors("Failed to malloc pipeline in create_pipeline", "");
		return (NULL);
	}
	pipeline->fd[0] = -2;
	pipeline->fd[1] = -2;
	pipeline->pid = -2;
	pipeline->tmp_fd = -2;

	return (pipeline);
}

int	initialize_ASTNode(t_ASTNode **node, char ***tokens)
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
	if (tokens && *tokens && is_redirection(*tokens))
	{
		(*node)->redir = create_redirection();
		if (parsing_others(tokens, &(*node)->redir, TRUE) == FAIL)
		{
			free_ASTNODE(node);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

t_ASTNode	*create_ASTNode(t_NodeType type, char ***tokens, \
			t_ASTNode *left, t_ASTNode *right, char **env)
{
	t_ASTNode	*ast;

	ast = (t_ASTNode *)malloc(sizeof(t_ASTNode));
	if (!ast)
	{
		log_errors("Failed to malloc node in create_ASTNode", "");
		return (NULL);
	}
	if (initialize_ASTNode(&ast, tokens) == FAIL)
	{
		free(ast);
		return (NULL);
	}
	ast->type = type;
	if (tokens && *tokens && **tokens)
	{
		ast->command = create_command(tokens, env);
		if (!ast->command)
			return (NULL);
	}
	ast->pipeline = create_pipeline();
	if (!ast->pipeline)
		return (NULL);
	if (ast->redir == NULL)
	{
		ast->redir = create_redirection();
		if (!ast->redir)
			return (NULL);
	}
	if (ast->command)
	{
		if (parsing_others(&ast->command->args, &ast->redir, FALSE) == FAIL)
		{
			free_ASTNODE(&ast);
			return (NULL);
		}
	}
	ast->left = left;
	ast->right = right;
	return (ast);
}

int	operation_parsing(char ***tokens, t_ASTNode **left_node, char **env)
{
	t_ASTNode *right_node;

	if (ft_strcmp(**tokens, "&&") == 0)
	{
		(*tokens)++;
		right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '&&' operation_parsing", \
					""));
		*left_node = create_ASTNode(NODE_AND, NULL, *left_node, \
					right_node, env);
	}
	else if (ft_strcmp(**tokens, "||") == 0)
	{
		(*tokens)++;
		right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '||' operation_parsing", \
					""));
		*left_node = create_ASTNode(NODE_OR, NULL, *left_node, \
					right_node, env);
	}
	else if (ft_strcmp(**tokens, "|") == 0)
	{
		(*tokens)++;
		right_node = create_ASTNode(NODE_COMMAND, tokens, NULL, NULL, env);
		if (!right_node)
			return (log_errors("NULL in RIGHT NODE : '|' operation_parsing", \
					""));
		*left_node = create_ASTNode(NODE_PIPE, NULL, *left_node, \
					right_node, env);
	}
	else
	{
		(*tokens)++;
	}
	return (SUCCESS);
}

t_ASTNode	*parse_to_Nodes(char **tokens, char **env)
{
	t_ASTNode	*left_node;

	if (!tokens || !*tokens)
		return (NULL);
	if (*tokens)
	{
		left_node = create_ASTNode(NODE_COMMAND, &tokens, NULL, NULL, env);
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

void	print_ASTNode(t_ASTNode *node, int depth)
{
	char **args;

	args = NULL;
	if (!node)
		return ;
	printf("\n");
	printf("Node Type: %d\n", node->type);
	if (node->command == NULL)
	{
		printf("Command: NULL\n");
	}
	else
	{
		printf("Command: %s\n", node->command->cmd);
	}
	if (node->command && node->command->args)
	{
		args = node->command->args;
		while (*args)
		{
			printf("cmd->args : %s\n", *node->command->args);
			args++;
		}
	}
	if (node->redir)
	{
		printf("Redirection details:\n");
		printf("infile: %d\n", node->redir->infile);
		printf("outfile: %d\n", node->redir->outfile);
		printf("filename: %s\n", node->redir->filename ? \
				node->redir->filename : "(NULL)");
		printf("direction_type: %d\n", node->redir->direction_type);
		printf("heredoc_limiter: %s\n", node->redir->heredoc_limiter ? \
				node->redir->heredoc_limiter : "(NULL)");
		printf("herestring_str: %s\n", node->redir->herestring_str ? \
				node->redir->herestring_str : "(NULL)");
		printf("dollar_vari: %s\n", node->redir->dollar_vari ? \
                node->redir->dollar_vari : "(NULL)");
	}
	print_ASTNode(node->left, depth + 1);
	print_ASTNode(node->right, depth + 1);
}
