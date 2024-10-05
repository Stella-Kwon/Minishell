/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astnode_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 04:29:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 19:21:05 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	}
	if (tokens && *tokens && is_redirection(*tokens))
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

// static void	print_redir_details(t_ASTNode *node)
// {
// 	printf("Redirection details:\n");
// 	printf("infile: %d\n", node->redir->infile);
// 	printf("outfile: %d\n", node->redir->outfile);
// 	printf("filename: %s\n", node->redir->filename ? \
// 			node->redir->filename : "(NULL)");
// 	printf("direction_type: %d\n", node->redir->direction_type);
// 	printf("heredoc_limiter: %s\n", node->redir->heredoc_limiter ? \
// 			node->redir->heredoc_limiter : "(NULL)");
// 	printf("herestring_str: %s\n", node->redir->herestring_str ? \
// 			node->redir->herestring_str : "(NULL)");
// }

void	print_astnode(t_ASTNode *node, int depth)
{
	char	**args;

	args = NULL;
	if (!node)
		return ;
	printf("\n");
	printf("Node Type: %d\n", node->type);
	if (node->command == NULL)
		printf("Command: NULL\n");
	else
		printf("Command: %s\n", node->command->cmd);
	if (node->command && node->command->args)
	{
		args = node->command->args;
		while (*args)
		{
			printf("cmd->args : %s\n", *node->command->args);
			args++;
		}
	}
	// if (node->redir)
	// 	print_redir_details(node);
	print_astnode(node->left, depth + 1);
	print_astnode(node->right, depth + 1);
}
