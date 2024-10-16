/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_herestr_parsing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:35:17 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 11:11:00 by sukwon           ###   ########.fr       */
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

int	heredoc_herestring_parsing(char ***args, t_Redirection **redirect, \
								int start)
{
	(void)start;
	if (ft_strcmp(**args, "<<") == 0)
	{
		(*args)++;
		if (set_heredoc(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else if (ft_strcmp(**args, "<<<") == 0)
	{
		(*args)++;
		if (set_herestring(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	return (SUCCESS);
}
