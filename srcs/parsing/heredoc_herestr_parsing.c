/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_herestr_parsing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:35:17 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/17 13:39:19 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_heredoc(t_Redirection **redirect, char *limiter)
{
	if (!(*redirect)->heredoc_limiter)
	{
		(*redirect)->heredoc_limiter = ft_calloc((*redirect)->heredoc_buffsize, \
												sizeof(char *));
		if (!(*redirect)->heredoc_limiter)
			return (log_errors("Failed malloc in set_heredoc", ""));
	}
	ft_realloc_double((*redirect)->heredoc_limiter, (*redirect)->heredoc_i, \
					&(*redirect)->heredoc_buffsize);
	(*redirect)->heredoc_limiter[(*redirect)->heredoc_i] = ft_strdup(limiter);
	if (!(*redirect)->heredoc_limiter[(*redirect)->heredoc_i])
		return (log_errors("Failed malloc [(*redirect)->heredoc_i++] in \
							set_heredoc", ""));
	(*redirect)->heredoc_i ++;
	return (SUCCESS);
}

int	set_herestring(t_Redirection **redirect, char *string)
{
	char	*str;

	str = ft_strjoin(string, "\n");
	(*redirect)->herestring_str = str;
	if (!(*redirect)->herestring_str)
		return (log_errors("Failed malloc in set_herestring", ""));
	return (SUCCESS);
}

int	herestring_action(int index, char ***args, \
t_Redirection **redirect, int start)
{
	if (start == TRUE)
		remove_arg(args, index);
	else
		(*args)++;
	if (set_herestring(redirect, **args) == FAIL)
		return (FAIL);
	if (start == TRUE)
		remove_arg(args, index);
	else
		(*args)++;
	return (SUCCESS);
}

int	heredoc_herestring_parsing(char ***args, t_Redirection **redirect, \
								int start)
{
	int	index;

	index = 0;
	if (ft_strcmp(**args, "<<") == 0)
	{
		if (start == TRUE)
			remove_arg(args, index);
		else
			(*args)++;
		if (set_heredoc(redirect, **args) == FAIL)
			return (FAIL);
		if (start == TRUE)
			remove_arg(args, index);
		else
			(*args)++;
	}
	else if (ft_strcmp(**args, "<<<") == 0)
	{
		if (herestring_action(index, args, redirect, start) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}
