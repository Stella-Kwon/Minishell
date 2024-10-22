/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:55:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 22:10:34 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_arg(char ***args, int index)
{
	free((*args)[index]);
	while ((*args)[index])
	{
		(*args)[index] = (*args)[index + 1];
		index++;
	}
	(*args)[index] = NULL;
}

// void	rm_store_arg(char ***args, int index, char ***new_args, int count)
// {
// 	int	i;
// 	int j;

// 	j = 0;
// 	i = 0;
// 	while (i < count)
//     {
//         if (i == index)
//         {
//             free((*args)[i]);
//         }
//         else
//         {
//             (*new_args)[j] = (*args)[i];
//             j++;
//         }
//         i++;
//     }
// 	(*new_args)[j] = NULL;
// }

// void remove_arg(char ***args, int index)
// {
// 	int count = 0;
// 	char **new_args;

// 	if (!args || !*args || index < 0 || !(*args)[index])
// 		return; 
// 	while ((*args)[count])
// 			count++ ;
// 	new_args = malloc((count) * sizeof(char *));
// 	if (!new_args)
// 		return ;
// 	rm_store_arg(args, index, &new_args, count);
// 	free(*args);
// 	*args = new_args;
// }

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
