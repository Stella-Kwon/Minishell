/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/24 22:17:46 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_redir_details(t_ASTNode *node)
{
	int	i;

	i = 0;
	printf("Redirection details:\n");
	printf("infile: %d\n", node->redir->infile);
	printf("outfile: %d\n", node->redir->outfile);
	printf("in_filename: %s\n", node->redir->in_filename ? 
			node->redir->in_filename : "(NULL)");
	printf("out_filename: %s\n", node->redir->out_filename ? 
			node->redir->out_filename : "(NULL)");
	printf("direction_type: %d\n", node->redir->direction_type);
	printf("(*node)->redir->heredoc_i  :%d\n", (node)->redir->heredoc_i);
	if (node->redir->heredoc_limiter)
	{
		while (i < node->redir->heredoc_i)
		{
			printf("heredoc_limiter: %s\n", node->redir->heredoc_limiter[i]);
			i++;
		}
	}
	printf("herestring_str: %s\n", node->redir->herestring_str ? 
			node->redir->herestring_str : "(NULL)");
}

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
			printf("cmd->args : %s\n", *args);
			size_t i = 0;
			while (i < ft_strlen(*args))
			{
				printf("cmd->args[%zu] : %c*\n", i, (*args)[i]);
				i++;
			}
			printf("length : %zu\n", ft_strlen(*args));
			args++;
		}
	}
	if (node->redir)
		print_redir_details(node);
	print_astnode(node->left, depth + 1);
	print_astnode(node->right, depth + 1);
}
