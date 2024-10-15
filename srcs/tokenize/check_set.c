/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/10 13:36:39 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *store_inside_set(char *tmp_start, char *tmp_end)
{
	char *tmp;
	ptrdiff_t offset;
	char *st;
	char *end;

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

void check_quotes_in_loop(t_Set *set, char ref, int *count)
{
	while (*set->tmp_end)
	{
		if (*set->tmp_end == ref)
			(*count)++;
		if ((*count % 2) == 0 && *set->tmp_end == ref)
		{
			while (ft_isspace(*(set->tmp_end)) == FALSE && *set->tmp_end)
				set->tmp_end++;
			break;
		}
		set->tmp_end++;
	}
}

static int check_quotes_and_depth(t_For_tokenize *tokenize, t_Set *set, char ref)
{
	int count;

	count = 0;
	if (set->depth == 0 || !set->single_quote || !set->double_quote)
	{
		if (*set->tmp_start == ref)
			count++;
		set->tmp_end = tokenize->start + 1;
		check_quotes_in_loop(set, ref, &count);
		tokenize->start = set->tmp_end;
	}
	return (SUCCESS);
}

char *check_set(t_For_tokenize *tokenize, char ref)
{
	t_Set set;

	set.depth = 0;
	set.single_quote = 0;
	set.double_quote = 0;
	set.tmp_start = tokenize->start;
	set.tmp_end = NULL;
	while (*set.tmp_start)
	{
		update_quotes_and_depth(&set.single_quote, &set.double_quote,
								&set.depth, *set.tmp_start);
		set.tmp_start++;
	}
	set.tmp_start = tokenize->start;
	if (check_quotes_and_depth(tokenize, &set, ref) == FAIL)
		return (NULL);
	tokenize->tokens[tokenize->token_count] = store_inside_set(set.tmp_start,
															   set.tmp_end);
	if (!tokenize->tokens[tokenize->token_count])
		return (NULL);
	return (tokenize->tokens[tokenize->token_count]);
}