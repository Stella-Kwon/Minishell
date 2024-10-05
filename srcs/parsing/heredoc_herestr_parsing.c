/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_herestr_parsing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:35:17 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 19:34:28 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_heredoc(t_Redirection **redirect, char *limiter)
{
	(*redirect)->heredoc_limiter = ft_strdup(limiter);
	if (!(*redirect)->heredoc_limiter)
		return (log_errors("Failed malloc in set_heredoc", ""));
	return (SUCCESS);
}

int	set_herestring(t_Redirection **redirect, char *string)
{
	(*redirect)->herestring_str = ft_strdup(string);
	if (!(*redirect)->herestring_str)
		return (log_errors("Failed malloc in set_herestring", ""));
	return (SUCCESS);
}

int	heredoc_herestring_parsing(char ***args, t_Redirection **redirect)
{
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
	else
	{
		return (2);
	}
	return (SUCCESS);
}
