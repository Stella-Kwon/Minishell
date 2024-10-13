/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/12 00:57:56 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int set_outfile(t_Redirection **redirect, char *filename, int direction_type)
{

	if (direction_type == REDIRECT_OUTPUT)
	{
		if ((*redirect)->outfile != -2)
			close((*redirect)->outfile);
		if (filename[0] == '.' && filename[1] == '/')
			check_directory_exist(redirect, filename);
		else
			(*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (direction_type == REDIRECT_APPEND)
	{
		if ((*redirect)->outfile != -2 )
			close((*redirect)->outfile);
		if (filename[0] == '.' && filename[1] == '/')
			check_directory_exist(redirect, filename);
		else
			(*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	return (SUCCESS);
}

static int set_infile(t_Redirection **redirect, char ***args, char *filename, int direction_type)
{
	if (direction_type == REDIRECT_INPUT)
	{
		if (put_last_open_infile(redirect, args, &filename) != SUCCESS)
			return (FAIL);
		if ((*redirect)->heredoc_limiter != NULL)
		{
			if ((*redirect)->tmp_infile != -2)
				close((*redirect)->tmp_infile);
			(*redirect)->tmp_infile = open(filename, O_RDONLY);
		}
		if ((*redirect)->herestring_str != NULL)
			free_one((void **)&(*redirect)->herestring_str);
		// printf("filename : %s, and (*redirect)->filename : %s\n", filename, (*redirect)->filename);
		if ((*redirect)->infile != -2 )
			close((*redirect)->infile);
		(*redirect)->infile = open(filename, O_RDONLY);
	}
	return (SUCCESS);
}

int set_redirection(t_Redirection **redirect, char ***args, int direction_type)
{
	char *filename;

	// printf("args in set redirection : %s\n", **args);
	if ((*redirect)->infile == -1 || (*redirect)->outfile == -1)
		return (SUCCESS);
	if ((*redirect)->infile != -1)
	{
		(*redirect)->filename = ft_strdup(**args);
		if (!(*redirect)->filename)
			return (log_errors("Failed in storing filename in set_redirection", ""));
		filename = (*redirect)->filename;
	}
	if (rm_quote_filename(redirect, args) != SUCCESS)
		return (log_errors("Failed in rm_quote_filename in set_redirection", ""));
	// printf("filename : %s, and (*redirect)->filename : %s\n", filename, (*redirect)->filename);
	if (set_outfile(redirect, filename, direction_type) != SUCCESS)
		return (FAIL);
	else if (set_infile(redirect, args, filename, direction_type) != SUCCESS)
		return (FAIL);
	// printf("fininshed setting well in set redirect");
	return (SUCCESS);
}

static int redirection_parsing_set(char ***args, t_Redirection **redirect)
{
	int direction_type;

	direction_type = get_direction_type(**args);
	if (direction_type == INVALID)
		return (log_errors("Invalid redirection type", ""));
	(*args)++;
	if (set_redirection(redirect, args, direction_type) == FAIL)
		return (FAIL);
	(*args)++;
	return (SUCCESS);
}

int redirection_parsing(char ***args, t_Redirection **redirect,
						int start)
{
	int i;

	i = 0;
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 ||
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
