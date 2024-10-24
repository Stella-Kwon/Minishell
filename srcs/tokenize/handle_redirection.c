/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 04:33:35 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_way(char *start, int *len)
{
	if (*(start + 1) == '<')
	{
		if (*(start + 2) && *(start + 2) == '<')
		{
			*len = 3;
			if (redirect_operation_error(start + 3) != SUCCESS)
				return (2);
		}
		else
		{
			*len = 2;
			if (redirect_operation_error(start + 2) != SUCCESS)
				return (2);
		}
	}
	else
		*len = 1;
	return (SUCCESS);
}

int	handle_input_redirection(t_For_tokenize *tokenize)
{
	int	len;
	int	i;

	len = 0;
	i = 1;
	while (ft_isspace(*(tokenize->start + i)))
		i++;
	if (!*(tokenize->start + i))
		return (handle_258_exitcode_print("`newline'"));
	if (*(tokenize->start + 1) == '&' || *(tokenize->start + 1) == '|')
	{
		if (!*(tokenize->start + 2))
			return (handle_258_exitcode_print("`newline'"));
		else if (redirect_operation_error(tokenize->start + 1) != SUCCESS)
			return (2);
	}
	if (handle_input_way(tokenize->start, &len) != SUCCESS)
		return (2);
	if (redirect_operation_error(tokenize->start + len) != SUCCESS)
		return (2);
	return (handle_token(tokenize, len));
}

static int	handle_output_way(char *start, int *len)
{
	if (*(start + 1) == '>')
	{
		*len = 2;
		if (redirect_operation_error(start + 2) != SUCCESS)
			return (2);
	}
	else
	{
		*len = 1;
		if (redirect_operation_error(start + 1) != SUCCESS)
			return (2);
	}
	return (SUCCESS);
}

int	handle_output_redirection(t_For_tokenize *tokenize)
{
	int	len;
	int	i;

	i = 1;
	while (ft_isspace(*(tokenize->start + i)))
		i++;
	if (!*(tokenize->start + i))
		return (handle_258_exitcode_print("`newline'"));
	if (*(tokenize->start + 1) == '&' || *(tokenize->start + 1) == '|')
	{
		if (!*((tokenize->start) + 2))
			return (handle_258_exitcode_print("`newline'"));
		else if (redirect_operation_error(tokenize->start + 2) != SUCCESS)
			return (2);
	}
	if (handle_output_way(tokenize->start, &len) != SUCCESS)
		return (2);
	return (handle_token(tokenize, len));
}
