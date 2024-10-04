/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:09:30 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:01:30 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_pipe_and_or(char **input, char **tokens, char **start,
		int *token_count)
{
	int	len;
	int	i;

	i = 0;
	if (*(*start + 1) == '|')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(*start + (++i))))
		;
	if (*(*start + len + i) == '\0')
	{
		if (check_operation_next(input, start, 0) == FAIL)
			return (FAIL);
	}
	return (handle_token(tokens, start, token_count, len));
}

int	handle_And_and_background(char **input, char **tokens,
		char **start, int *token_count)
{
	int	len;
	int	i;

	i = 0;
	if (*(*start + 1) == '&')
		len = 2;
	else
		len = 1;
	while (ft_isspace(*(*start + (++i))))
		;
	if (*(*start + len + i) == '\0')
	{
		if (check_operation_next(input, start, 0) == FAIL)
			return (FAIL);
	}
	return (handle_token(tokens, start, token_count, len));
}

int	redirect_operation_error(char *start)
{
	while (ft_isspace(*start))
		(start)++;
	if (*(start) == '\0')
		return (handle_258_exitcode_print("newline"));
	if (*(start) == '|')
	{
		if (*(start + 1) == '|')
			return (handle_258_exitcode_print("'||'"));
		else
			return (handle_258_exitcode_print("'|'"));
	}
	else if (*(start) == '&')
	{
		if (*(start + 1) == '&')
			return (handle_258_exitcode_print("'&&'"));
		else
			return (handle_258_exitcode_print("'&'"));
	}
	else if (*(start) == '<')
	{
		if (*(start + 1) == '<')
			return (handle_258_exitcode_print("'<<'"));
		else if (*(start + 2) == '<')
			return (handle_258_exitcode_print("'<<<'"));
		else
			return (handle_258_exitcode_print("'<'"));
	}
	else if (*(start) == '>')
	{
		if (*(start + 1) == '>')
			return (handle_258_exitcode_print("'>>'"));
		else
			return (handle_258_exitcode_print("'>'"));
	}
	return (SUCCESS);
}

int	handle_input_redirection(char **tokens, char **start, int *token_count)
{
	int	len;
	int	i;

	i = 1;
	while (ft_isspace(*(*start + i)))
		i++;
	if (!*(*start + i))
		return (handle_258_exitcode_print("newline"));
	if (*(*start + 1) == '&')
	{
		if (!*(*(start) + 2))
			return (handle_258_exitcode_print("newline"));
		else if (redirect_operation_error(*(start) + 2) == FAIL)
			return (FAIL);
	}
	if (*(*start + 1) == '<')
	{
		if (*(*start + 2) == '<')
		{
			len = 3;
			if (redirect_operation_error(*(start) + 3) == FAIL)
				return (FAIL);
		}
		else
		{
			len = 2;
			if (redirect_operation_error(*(start) + 2) == FAIL)
				return (FAIL);
		}
	}
	else
		len = 1;
	return (handle_token(tokens, start, token_count, len));
}

int	handle_output_redirection(char **tokens, char **start, int *token_count)
{
	int	len;
	int	i;

	i = 1;
	while (ft_isspace(*(*start + i)))
		i++;
	if (!*(*start + i))
		return (handle_258_exitcode_print("newline"));
	if (*(*start + 1) == '&')
	{
		if (!*(*(start) + 2))
			return (handle_258_exitcode_print("newline"));
		else if (redirect_operation_error(*(start) + 2) == FAIL)
			return (FAIL);
	}
	if (*(*start + 1) == '>')
	{
		len = 2;
		if (redirect_operation_error(*(start) + 2) == FAIL)
			return (FAIL);
	}
	else
	{
		len = 1;
		if (redirect_operation_error(*(start) + 1) == FAIL)
			return (FAIL);
	}
	return (handle_token(tokens, start, token_count, len));
}

int	handle_token(char **tokens, char **start, int *token_count, int len)
{
	while (ft_isspace(**start))
		(*start)++;
	tokens[*token_count] = ft_strndup(*start, len);
	if (!tokens[*token_count])
	{
		all_free(&tokens);
		log_errors("Failed to ft_strndup in tokenize_input", "");
		return (FAIL);
	}
	(*token_count)++;
	*start += len;
	return (SUCCESS);
}

int	handle_set(char **tokens, char **start, int *token_count, char **input, char ref)
{
	if (**start == ')')
		return (handle_258_exitcode_print(")"));
	tokens[*token_count] = check_set(input, start, tokens, token_count, ref);
	if (!tokens[*token_count])
	{
		all_free(&tokens);
		log_errors("Failed to handle set token in tokenize_input", "");
		return (FAIL);
	}
	(*token_count)++;
	if (**start == '(')
	{
		while (ft_isspace(**start))
			(*start)++;
		if (!**start)
			return (handle_258_exitcode_print(")"));
	}
	return (SUCCESS);
}
