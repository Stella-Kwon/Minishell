/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:35:17 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/17 13:39:19 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_heredoc(t_Redirection **redirect, char *limiter)
{
	if (!(*redirect)->heredoc_limiter)
	{
		(*redirect)->heredoc_limiter = ft_calloc((*redirect)->heredoc_buffsize,
												sizeof(char *));
		if (!(*redirect)->heredoc_limiter)
			return (log_errors("Failed malloc in set_heredoc", ""));
	}
	ft_realloc_double((*redirect)->heredoc_limiter, (*redirect)->heredoc_i,
					&(*redirect)->heredoc_buffsize);
	(*redirect)->heredoc_limiter[(*redirect)->heredoc_i] = ft_strdup(limiter);
	if (!(*redirect)->heredoc_limiter[(*redirect)->heredoc_i])
		return (log_errors("Failed malloc [(*redirect)->heredoc_i++] in \
							set_heredoc",
						""));
	(*redirect)->heredoc_i++;
	(*redirect)->last_stdin_type = 2;
	return (SUCCESS);
}
