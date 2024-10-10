/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/10 21:46:17 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_redirection(t_Redirection **redirect, char ***args, \
		int direction_type)
{
	char *filename;

	filename = **args;
	if (put_last_open_file(redirect, args) != SUCCESS)
	{
		printf("filename : %s\n", (*redirect)->filename);
		return (FAIL);
	}
	if (direction_type == REDIRECT_OUTPUT)
		(*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (direction_type == REDIRECT_APPEND)
		(*redirect)->outfile = open(filename, O_WRONLY | \
				O_CREAT | O_APPEND, 0644);
	else if (direction_type == REDIRECT_INPUT)
	{
		if ((*redirect)->heredoc_limiter != NULL)
			(*redirect)->tmp_infile = open(filename, O_RDONLY);
		if ((*redirect)->herestring_str != NULL)
			free_one((void **)&(*redirect)->herestring_str);
		(*redirect)->infile = open(filename, O_RDONLY);
	}
	if ((*redirect)->outfile == -1 || (*redirect)->infile == -1)
		return (log_errors((*redirect)->filename, \
				"Failed in opening file in set_redirection"));
	return (SUCCESS);
}

static int	redirection_parsing_set(char ***args, t_Redirection **redirect)
{
	int	direction_type;

	direction_type = get_direction_type(**args);
	if (direction_type == INVALID)
		return (log_errors("Invalid redirection type", ""));
	(*args)++;
	if (set_redirection(redirect, args, direction_type) == FAIL)
		return (FAIL);
	(*args)++;
	return (SUCCESS);
}

int	redirection_parsing(char ***args, t_Redirection **redirect,	\
						int start)
{
	int	i;

	i = 0;
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || \
		ft_strcmp(**args, "<") == 0)
	{
		if (redirection_parsing_set(args, redirect) == FAIL)
			return (FAIL);
	}
	else
	{
		i = heredoc_herestring_parsing(args, redirect, start);
		if (i == FAIL)
			return (FAIL);
		else if (i == 2)
			return (2);
	}
	return (SUCCESS);
}
