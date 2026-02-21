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

int heredoc_herestring_parsing(char ***args, t_Redirection *redirect, t_TokenizeResult *result)
{
	if (ft_strcmp(**args, "<<") == 0)
	{
		(*args)++;
		if (set_heredoc(redirect, **args, result) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else if (ft_strcmp(**args, "<<<") == 0)
	{
		if (herestring_action(args, redirect) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}

int set_heredoc(t_Redirection *redirect, char *limiter, t_TokenizeResult *result)
{
	int local_index;
	int max_size;

	if (!redirect->heredoc_limiter)
	{
		max_size = result ? result->heredoc_count : HEREDOC_BUFFSIZE;
		redirect->heredoc_limiter = ft_calloc(max_size + 1, sizeof(char *));
		if (!redirect->heredoc_limiter)
			return (log_errors("Failed malloc in set_heredoc", ""));
		if (result)
		{
			redirect->heredoc_body = ft_calloc(max_size + 1, sizeof(char *));
			if (!redirect->heredoc_body)
				return (log_errors("Failed malloc heredoc_body in set_heredoc", ""));
		}
		redirect->heredoc_buffsize = max_size;
	}
	local_index = redirect->heredoc_i;
	if (local_index >= redirect->heredoc_buffsize)
		return (log_errors("Heredoc index exceeds buffer size in set_heredoc", ""));
	redirect->heredoc_limiter[local_index] = ft_strdup(limiter);
	if (!redirect->heredoc_limiter[local_index])
		return (log_errors("Failed malloc [redirect->heredoc_i++] in set_heredoc", ""));
	if (result && result->heredoc_bodies && result->current_heredoc_index < result->heredoc_count)
		redirect->heredoc_body[local_index] = result->heredoc_bodies[result->current_heredoc_index];
	redirect->heredoc_i++;
	if (result)
		result->current_heredoc_index++;
	redirect->last_stdin_type = 2;
	return (SUCCESS);
}
