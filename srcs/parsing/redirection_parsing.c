/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 19:18:19 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return (redir);
}

int	set_redirection(t_Redirection **redirect, char *filename, \
		int direction_type)
{
	(*redirect)->filename = ft_strdup(filename);
	if (!(*redirect)->filename)
		return (log_errors("Failed in storing filename in " \
				"set_redirection", ""));
	if (direction_type == REDIRECT_OUTPUT)
		(*redirect)->outfile = open(filename, O_WRONLY | \
				O_CREAT | O_TRUNC, 0644);
	else if (direction_type == REDIRECT_APPEND)
		(*redirect)->outfile = open(filename, O_WRONLY | \
				O_CREAT | O_APPEND, 0644);
	else if (direction_type == REDIRECT_INPUT)
		(*redirect)->infile = open(filename, O_RDONLY);
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
	if (access(**args, F_OK) == -1)
	{
		log_errors("File not found", "");
		return (FAIL);
	}
	if (set_redirection(redirect, **args, direction_type) == FAIL)
		return (FAIL);
	(*args)++;
	return (SUCCESS);
}

static int	handle_redirection_found(char ***args)
{
	while (**args && (ft_strcmp(**args, ">") == 0 || \
			ft_strcmp(**args, ">>") == 0 || ft_strcmp(**args, "<") == 0))
	{
		(*args)++;
		if (**args)
			(*args)++;
	}
	return (SUCCESS);
}

int	redirection_parsing(char ***args, t_Redirection **redirect,	\
						int *redirection_found)
{
	int	i;

	i = 0;
	if (*redirection_found == 1)
		return (handle_redirection_found(args));
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || \
		ft_strcmp(**args, "<") == 0)
	{
		if (redirection_parsing_set(args, redirect) == FAIL)
			return (FAIL);
		*redirection_found = 1;
	}
	else
	{
		i = heredoc_herestring_parsing(args, redirect);
		if (i == FAIL)
			return (FAIL);
		else if (i == 2)
			return (2);
	}
	return (SUCCESS);
}