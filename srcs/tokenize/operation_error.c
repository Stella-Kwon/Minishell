/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/23 16:55:04 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	operation_error_rest(char *start)
{
	if (*(start) == '<')
	{
		if (*(start + 1) == '<')
			return (handle_258_exitcode_print("`<<'"));
		else if (*(start + 2) && * (start + 2) == '<')
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

void rm_store_str(t_Set *set, char ref, char **result, int len)
{
	int i;
	int j;
	char *str;

	i = 0;
	j = 0;
	str = set->tmp_start;
	while (str[i] != ref)
		(*result)[j++] = str[i++];
	if (str[i] == ref)
		i++;
	while (str[--len] != ref && len > i)
		;
	while (i < len)
		(*result)[j++] = str[i++];
	while (str[++len])
		(*result)[j++] = str[len];
	(*result)[j] = '\0';
}

char	*check_quote_store(t_Set *set, char ref)
{
	int	len;
	char *result;

	result = NULL;
	if (set->depth == 0 || !set->single_quote || !set->double_quote)
	{
		len = ft_strlen(set->tmp_start);
		result = (char *)malloc(sizeof(char) * (len + 1));
		if (!result)
			return NULL;
		rm_store_str(set, ref, &result, len);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error: unclosed quotes\n", 2);
	}
	return (result);
}

char *rm_quotes(char *str)
{
	t_Set	set;
	char	ref;
	char	*result;

	initialize_set(str, &set);
	set_ref_and_tmp_start(str, &set, &ref);
	result = check_quote_store(&set, ref);
	if (!result)
		return (NULL);
	return (result);
}