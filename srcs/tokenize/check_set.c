/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/05 08:42:19 by hlee-sun         ###   ########.fr       */
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

static int	check_quotes_and_depth(t_For_tokenize *tokenize, t_Set *set, \
									char ref)
{
	if (set->depth > 0 || set->single_quote || set->double_quote)
	{
		readline_again(tokenize, set);
		if (!tokenize->input)
			return (FAIL);
		tokenize->start = tokenize->input + strlen(tokenize->input);
	}
	else
	{
		set->tmp_end = tokenize->start + 1;
		while (*set->tmp_end && * set->tmp_end != ref)
			set->tmp_end++;
		while (*set->tmp_end)
		{
			set->tmp_end++;
			if (ft_isspace(*set->tmp_end))
				break;
		}
		tokenize->start = set->tmp_end + 1;
	}
	return (SUCCESS);
}

static void	check_set_start(t_Set *set, t_For_tokenize *tokenize)
{
	if (*set->tmp_start == '(' || *set->tmp_end == ')')
	{
		tokenize->tokens[tokenize->token_count] = ft_strdup("(");
		tokenize->tokens[tokenize->token_count + 1] = ft_strdup(")");
		tokenize->token_count += 2;
		set->tmp_start += 1;
		set->tmp_end -= 1;
	}
}

char	*check_set(t_For_tokenize *tokenize, char ref)
{
	t_Set	set;

	set.depth = 0;
	set.single_quote = 0;
	set.double_quote = 0;
	set.tmp_start = tokenize->start;
	set.tmp_end = NULL;
	printf("where : %s", tokenize->start);
	while (*set.tmp_start)
	{
		update_quotes_and_depth(&set.single_quote, &set.double_quote,
			&set.depth, *set.tmp_start);
		set.tmp_start++;
	}
	set.tmp_start = tokenize->start;
	if (check_quotes_and_depth(tokenize, &set, ref) == FAIL)
		return (NULL);
	check_set_start(&set, tokenize);
	tokenize->tokens[tokenize->token_count] = store_inside_set(set.tmp_start, \
																set.tmp_end);
	if (!tokenize->tokens[tokenize->token_count])
		return (NULL);
	return (tokenize->tokens[tokenize->token_count]);
}
