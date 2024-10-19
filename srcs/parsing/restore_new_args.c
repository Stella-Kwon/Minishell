/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_new_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/12 00:57:56 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	check_order(char ***args, int *origin_i)
{
	if (strcmp((*args)[*origin_i - 1], "<<<") == 0 || \
		strcmp((*args)[*origin_i - 1], "<<") == 0 || \
		strcmp((*args)[*origin_i - 1], ">") == 0 || \
		strcmp((*args)[*origin_i - 1], ">>") == 0)
	{
		(*origin_i)++;
	}
	else if (strcmp((*args)[*origin_i], "<<<") == 0 || \
			strcmp((*args)[*origin_i], "<<") == 0 || \
			strcmp((*args)[*origin_i], ">") == 0 || \
			strcmp((*args)[*origin_i], ">>") == 0)
	{
		(*origin_i) += 2;
	}
}

static void	loop_for_each(char ***args, int *origin_i)
{
	while ((*args)[*origin_i])
	{
		if (strcmp((*args)[*origin_i - 1], "<") == 0 || \
			strcmp((*args)[*origin_i], "<") == 0)
			infile_in_row(args, origin_i);
		if (!(*args)[*origin_i])
			break ;
		check_order(args, origin_i);
		if (!(*args)[*origin_i])
			break ;
		if (access((*args)[*origin_i], F_OK) != 0)
		{
			if (strcmp((*args)[*origin_i], "<<<") == 0 || \
				strcmp((*args)[*origin_i], "<") == 0 || \
				strcmp((*args)[*origin_i], "<<") == 0 || \
				strcmp((*args)[*origin_i], ">") == 0 || \
				strcmp((*args)[*origin_i], ">>") == 0)
				continue ;
			else
				break ;
		}
	}
}

static void	check_file_access(char ***args, int *origin_i)
{
	if (strcmp((*args)[*origin_i], "<") == 0 || \
		strcmp((*args)[*origin_i], "<<<") == 0 || \
		strcmp((*args)[*origin_i], "<<") == 0 || \
		strcmp((*args)[*origin_i], ">") == 0 || \
		strcmp((*args)[*origin_i], ">>") == 0)
	{
		(*origin_i)++;
		loop_for_each(args, origin_i);
	}
}

int	restore_new_args(char ***args, t_rm_args *rm)
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
			return (log_errors("Failed to malloc new_args[i] in \
								rm_arg function", ""));
		rm->origin_i++;
		rm->i++;
		rm->new_args = ft_realloc_double(rm->new_args, rm->i, &rm->buffersize);
		if (!rm->new_args)
			return (log_errors("Failed to realloc new_args in \
								create_command", ""));
	}
	if (!(*args)[rm->origin_i])
		(*args)[rm->origin_i] = NULL;
	return (SUCCESS);
}
