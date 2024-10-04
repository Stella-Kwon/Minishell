/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:59:41 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*store_inside_set(char *tmp_start, char *tmp_end)
{
	char		*tmp;
	ptrdiff_t	offset;
	char		*st;
	char		*end;

	if (!tmp_start || !tmp_end || tmp_start > tmp_end)
	{
		log_errors("Invalid pointers in store_inside_set", "");
		return (NULL);
	}
	st = tmp_start;
	end = tmp_end;
	offset = end - st;
	tmp = ft_strndup(tmp_start, offset + 1);
	if (!tmp)
	{
		log_errors("Failed to allocate memory in store_inside_set", "");
		return (NULL);
	}
	return (tmp);
}

void	update_quotes_and_depth(int *single_quote, int *double_quote,
	int *depth, char c)
{
	if (c == '\'' && !*double_quote)
		*single_quote = !*single_quote;
	else if (c == '"' && !*single_quote)
		*double_quote = !*double_quote;
	else if (!*single_quote && !*double_quote)
	{
		if (c == '(')
			(*depth)++;
		else if (c == ')')
			(*depth)--;
	}
}

static int	check_quotes_and_depth(char **input, char **start, \
								t_Set *set, char ref)
{
	if (set->depth > 0 || set->single_quote || set->double_quote)
	{
		readline_again(input, set, start);
		if (!input)
			return (FAIL);
		*start = *input + strlen(*input);
	}
	else
	{
		set->tmp_end = *start + 1;
		while (*set->tmp_end != ref)
			set->tmp_end++;
		*start = set->tmp_end + 1;
	}
	return (SUCCESS);
}

static void	check_set_start(t_Set *set, char **tokens, int *token_count)
{
	if (*set->tmp_start == '\'' || *set->tmp_start == '"')
	{
		set->tmp_start += 1;
		set->tmp_end -= 1;
	}
	if (*set->tmp_start == '(' || *set->tmp_end == ')')
	{
		tokens[*token_count] = ft_strdup("(");
		tokens[(*token_count) + 1] = ft_strdup(")");
		*token_count += 2;
		set->tmp_start += 1;
		set->tmp_end -= 1;
	}
}

char	*check_set(char **input, char **start, char **tokens, \
				int *token_count, char ref)
{
	t_Set	set;

	set.depth = 0;
	set.single_quote = 0;
	set.double_quote = 0;
	set.tmp_start = *start;
	set.tmp_end = NULL;
	while (*set.tmp_start)
	{
		update_quotes_and_depth(&set.single_quote, &set.double_quote,
			&set.depth, *set.tmp_start);
		set.tmp_start++;
	}
	set.tmp_start = *start;
	if (check_quotes_and_depth(input, start, &set, ref) == FAIL)
		return (NULL);
	check_set_start(&set, tokens, token_count);
	tokens[*token_count] = store_inside_set(set.tmp_start, set.tmp_end);
	if (!tokens[*token_count])
		return (NULL);
	return (tokens[*token_count]);
}
