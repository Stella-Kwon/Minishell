/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:34 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int handle_whitespace(char **start)
{
	while (ft_isspace(**start))
		(*start)++;
	return (SUCCESS);
}

int handle_special_tokens(t_For_tokenize *tokenize)
{
	if (*tokenize->start == '\'')
		return (handle_set(tokenize, '\''));
	else if (*tokenize->start == '"')
		return (handle_set(tokenize, '"'));
	else if (*tokenize->start == '(')
		return (handle_set(tokenize, '('));
	else if (*tokenize->start == '<')
		return (handle_input_redirection(tokenize));
	else if (*tokenize->start == '>')
		return (handle_output_redirection(tokenize));
	else if (*tokenize->start == '|')
		return (handle_pipe_and_or(tokenize));
	else if (*tokenize->start == '&')
		// return (handle_and_and_background(tokenize));
		return (handle_and(tokenize));
	else if (ft_isspace(*(tokenize->start)))
	{
		handle_whitespace(&tokenize->start);
		return (SUCCESS);
	}
	else
	{
		if (store_str(tokenize) != SUCCESS)
			return (FAIL);
	}
	return (SUCCESS);
}
