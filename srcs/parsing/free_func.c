/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:55:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 17:37:49 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	remove_arg(char ***args, int index)
// {
// 	if (!args || !*args || index < 0 || !(*args)[index])
// 		return;
// 	free((*args)[index]);
// 	while ((*args)[index])
// 	{
// 		(*args)[index] = (*args)[index + 1];
// 		index++;
// 	}
// 	(*args)[index] = NULL;
// }


void remove_arg(char ***args, int index)
{
	int count = 0;
	char **new_args;
	int i = 0;
	int j = 0;

	if (!args || !*args || index < 0 || !(*args)[index])
		return; 
	while ((*args)[count])
		count++ ;
	new_args = malloc((count - 1) * sizeof(char *));
	if (!new_args)
		return ;
	while (i < count && (*args)[i])
	{
		if (i != index)
		{
			new_args[j] = (*args)[i];
			j++;
		}
		else
			free((*args)[i]);
		i++;
	}
	new_args[j] = NULL;
	free(*args);
	*args = new_args;
}

void	free_command(t_Command **res)
{
	if (*res)
	{
		if ((*res)->cmd)
			free_one((void **)&(*res)->cmd);
		if ((*res)->args)
			all_free(&(*res)->args);
		free_one((void **)&(*res));
	}
}

void	free_redirection(t_Redirection **redir)
{
	if (!(*redir))
		return ;
	if ((*redir)->in_filename)
		free_one((void **)&(*redir)->in_filename);
	if ((*redir)->out_filename)
		free_one((void **)&(*redir)->out_filename);
	if ((*redir)->heredoc_limiter)
		all_free(&(*redir)->heredoc_limiter);
	if ((*redir)->herestring_str)
		free_one((void **)&(*redir)->herestring_str);
	free_one((void **)&(*redir));
}

void	free_astnode(t_ASTNode **node)
{
	if (*node)
	{
		if ((*node)->command)
		{
			free_command(&(*node)->command);
			// printf("freed cmd\n");
		}
		if ((*node)->pipeline)
		{
			free_one((void **)&(*node)->pipeline);
			// printf("freed pipe\n");
		}

		if ((*node)->redir)
		{
			free_redirection(&(*node)->redir);
			// printf("freed redir\n");
		}

		if ((*node)->left)
		{
			free_astnode(&(*node)->left);
			// printf("freed left\n");
		}
			
		if ((*node)->right)
		{
			free_astnode(&(*node)->right);
			// printf("freed right\n");
		}
			
		free_one((void **)&(*node));
		// printf("----------free whole node----------\n");
	}
}
