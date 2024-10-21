/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 04:33:16 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	operation_error_rest(char *start)
{
	if (*(start) == '<')
	{
		if (*(start + 1) == '<')
			return (handle_258_exitcode_print("`<<'"));
		else if (*(start + 2) == '<')
			return (handle_258_exitcode_print("`<<<'"));
		else
			return (handle_258_exitcode_print("`<'"));
	}
	else if (*(start) == '>')
	{
		if (*(start + 1) == '>')
			return (handle_258_exitcode_print("`>>'"));
		else
			return (handle_258_exitcode_print("`>'"));
	}
	return (SUCCESS);
}

int	redirect_operation_error(char *start)
{
	while (ft_isspace(*start))
		(start)++;
	if (*(start) == '\0')
		return (handle_258_exitcode_print("`newline'"));
	if (*(start) == '|')
	{
		if (*(start + 1) == '|')
			return (handle_258_exitcode_print("`||'"));
		else
			return (handle_258_exitcode_print("`|'"));
	}
	else if (*(start) == '&')
	{
		if (*(start + 1) == '&')
			return (handle_258_exitcode_print("`&&'"));
		else
			return (handle_258_exitcode_print("`&'"));
	}
	else if (operation_error_rest(start) != SUCCESS)
		return (2);
	return (SUCCESS);
}
