/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 20:44:29 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	set_outfile(t_Redirection **redirect, char ***args, \
						int direction_type)
{
	if (direction_type == REDIRECT_OUTPUT)
	{
		if (rm_quote_filename(redirect, args, &(*redirect)->out_filename) \
								!= SUCCESS)
			return (log_errors("Failed in rm_quote_filename in \
								set_redirection", ""));
		if ((*redirect)->outfile != -2)
			close((*redirect)->outfile);
		(*redirect)->outfile = open((*redirect)->out_filename, \
									O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (direction_type == REDIRECT_APPEND)
	{
		if (rm_quote_filename(redirect, args, &(*redirect)->out_filename) \
								!= SUCCESS)
			return (log_errors("Failed in rm_quote_filename in \
								set_redirection", ""));
		if ((*redirect)->outfile != -2)
			close((*redirect)->outfile);
		(*redirect)->outfile = open((*redirect)->out_filename, \
									O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	(*redirect)->errno_out = errno;
	// printf("(*redirect)->errno_out :%d\n", (*redirect)->errno_out);
	return (SUCCESS);
}

static int	set_infile(t_Redirection **redirect, char ***args, \
						int direction_type)
{
	if (direction_type == REDIRECT_INPUT)
	{
		if (put_last_open_infile(redirect, args, &(*redirect)->in_filename) \
								!= SUCCESS)
			return (FAIL);
		if ((*redirect)->heredoc_limiter && (*redirect)->heredoc_limiter[0])
		{
			if ((*redirect)->heredoc_infile != -2)
				close((*redirect)->heredoc_infile);
			(*redirect)->heredoc_infile = -2;
		}
		if ((*redirect)->herestring_str != NULL)
			free_one((void **)&(*redirect)->herestring_str);
		if ((*redirect)->infile != -2)
			close((*redirect)->infile);
		(*redirect)->infile = open((*redirect)->in_filename, O_RDONLY);
	}
	(*redirect)->errno_in = errno;
	return (SUCCESS);
}

int	set_redirection(t_Redirection **redirect, char ***args, int direction_type)
{
	if ((*redirect)->infile == -1 || (*redirect)->outfile == -1)
		return (SUCCESS);
	if (set_outfile(redirect, args, direction_type) != SUCCESS)
		return (FAIL);
	if (set_infile(redirect, args, direction_type) != SUCCESS)
		return (FAIL);
	return (SUCCESS);
}

static int	redirection_parsing_set(char ***args, \
t_Redirection **redirect, int start)
{
	int	index;
	int	direction_type;

	index = 0;
	direction_type = get_direction_type(**args);
	if (direction_type == INVALID)
		return (log_errors("Invalid redirection type", ""));
	if (start == TRUE)
		remove_arg(args, index);
	else
		(*args)++;
	if (set_redirection(redirect, args, direction_type) == FAIL)
		return (FAIL);
	if (start == TRUE)
		remove_arg(args, index);
	else
		(*args)++;
	return (SUCCESS);
}

int	redirection_parsing(char ***args, t_Redirection **redirect, int start)
{
	int	i;

	i = 0;
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || \
		ft_strcmp(**args, "<") == 0)
	{
		if (redirection_parsing_set(args, redirect, start) == FAIL)
			return (FAIL);
	}
	else
	{
		i = heredoc_herestring_parsing(args, redirect, start);
		if (i == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
