/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astnode_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 04:29:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/12 20:08:06 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_redirection(char **token)
{
	if (ft_strcmp(*token, "<") == 0 || ft_strcmp(*token, "<<") == 0 ||
		ft_strcmp(*token, "<<<") == 0 || ft_strcmp(*token, ">") == 0 ||
		ft_strcmp(*token, ">>") == 0)
		return (TRUE);
	return (FALSE);
}

static void	infile_in_row(char ***args, int *origin_i)
{
	while ((*args)[*origin_i])
	{
		if (strcmp((*args)[*origin_i - 1], "<") == 0 && (*args)[*origin_i])
		{
			(*origin_i)++;
		}
		else if (strcmp((*args)[*origin_i], "<") == 0 && (*args)[*origin_i + 1])
			(*origin_i) += 2;
		else if (access((*args)[*origin_i], F_OK) != 0)
			break ;
		else if (access((*args)[*origin_i], F_OK) == 0)
			(*origin_i)++;
	}
}

static void	check_file_access(char ***args, int *origin_i)
{
	int	flag;

	if (strcmp((*args)[*origin_i], "<") == 0 || \
		strcmp((*args)[*origin_i], "<<<") == 0 || \
		strcmp((*args)[*origin_i], "<<") == 0 || \
		strcmp((*args)[*origin_i], ">") == 0 || \
		strcmp((*args)[*origin_i], ">>") == 0)
	{
		(*origin_i)++;
		while ((*args)[*origin_i])
		{
			flag = 0;
			infile_in_row(args, origin_i);
			if (!(*args)[*origin_i])
				break ;
			if (strcmp((*args)[*origin_i], "<<<") == 0 || \
				strcmp((*args)[*origin_i], "<<") == 0 || \
				strcmp((*args)[*origin_i], ">") == 0 || \
				strcmp((*args)[*origin_i], ">>") == 0)
			{
				(*origin_i)++;
				flag = 1;
			}
			if (access((*args)[*origin_i], F_OK) != 0)
			{
				if (flag)
					(*origin_i)++;
				break ;
			}
			(*origin_i)++;
		}
	}
}

static int	restore_new_args(char ***args, t_rm_args *rm)
{
	while ((*args)[rm->origin_i])
	{
		check_file_access(args, &rm->origin_i);
		if (!(*args)[rm->origin_i])
		{
			(*args)[rm->origin_i] = NULL;
			return (SUCCESS);
		}
		rm->new_args[rm->i] = ft_strdup((*args)[rm->origin_i]);
		if (!rm->new_args[rm->i])
			return (log_errors("Failed to malloc \
					new_args[i] in rm_arg function", ""));
		rm->origin_i++;
		rm->i++;
		rm->new_args = ft_realloc_double(rm->new_args, rm->i, &rm->buffersize);
		if (!rm->new_args)
			return (log_errors("Failed to realloc \
					 new_args in create_command", ""));
	}
	if (!(*args)[rm->origin_i])
		(*args)[rm->origin_i] = NULL;
	return (SUCCESS);
}

int	remove_args_after_redirection(char ***args)
{
	t_rm_args	rm;

	rm.buffersize = BUFFER_SIZE;
	rm.i = 0;
	rm.origin_i = 0;
	rm.new_args = ft_calloc(rm.buffersize, sizeof(char *));
	if (!rm.new_args)
		return (log_errors("Failed to malloc \
				new_args in remove_args function", ""));
	if (restore_new_args(args, &rm) != SUCCESS)
		return (FAIL);
	if (*args)
	{
		all_free(args);
	}
	*args = rm.new_args;
	return (SUCCESS);
}

// static void	print_redir_details(t_ASTNode *node)
// {
// 	printf("Redirection details:\n");
// 	printf("infile: %d\n", node->redir->infile);
// 	printf("outfile: %d\n", node->redir->outfile);
// 	printf("filename: %s\n", node->redir->filename ? node->redir->filename : "(NULL)");
// 	printf("direction_type: %d\n", node->redir->direction_type);
// 	printf("heredoc_limiter: %s\n", node->redir->heredoc_limiter ? node->redir->heredoc_limiter : "(NULL)");
// 	printf("herestring_str: %s\n", node->redir->herestring_str ? node->redir->herestring_str : "(NULL)");
// }

// void print_astnode(t_ASTNode *node, int depth)
// {
// 	char **args;

// 	args = NULL;
// 	if (!node)
// 		return;
// 	printf("\n");
// 	printf("Node Type: %d\n", node->type);
// 	if (node->command == NULL)
// 		printf("Command: NULL\n");
// 	else
// 		printf("Command: %s\n", node->command->cmd);
// 	if (node->command && node->command->args)
// 	{
// 		args = node->command->args;
// 		while (*args)
// 		{
// 			printf("cmd->args : %s\n", *args);
// 			args++;
// 		}
// 	}
// 	if (node->redir)
// 		print_redir_details(node);
// 	print_astnode(node->left, depth + 1);
// 	print_astnode(node->right, depth + 1);
// }
