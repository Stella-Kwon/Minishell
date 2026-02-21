/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/17 21:59:27 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void initialize_set(char *start, t_Set *set)
{
	set->single_quote = 0;
	set->double_quote = 0;
	set->parenthesis = 0;
	set->tmp_start = start;
	set->tmp_end = NULL;
}

void check_pairs(int *single_quote, int *double_quote,
				 int *prarenthesis, char c)
{
	if (c == '\'' && !*double_quote)
		*single_quote = !*single_quote;
	else if (c == '"' && !*single_quote)
		*double_quote = !*double_quote;
	else if (!*single_quote && !*double_quote)
	{
		if (c == '(')
			(*prarenthesis)++;
		else if (c == ')')
			(*prarenthesis)--;
	}
}

static void check_quotes_in_loop(t_Set *set, char ref, int *count)
{
	while (*set->tmp_end)
	{
		if (*set->tmp_end == ref)
		{
			(*count)++;
			if ((*count % 2) == 0 && (*(set->tmp_end + 1) == '\0'))
				break;
		}
		if ((*count % 2) == 0 && (*set->tmp_end == ' ' || *set->tmp_end == '\t'))
			break;
		set->tmp_end++;
	}
}

static int call_readline_again(t_For_tokenize *tokenize, t_Set *set, char ref)
{
	int count;
	int ret;

	count = 0;
	if (set->parenthesis == 0 && !set->single_quote && !set->double_quote)
	{
		set->tmp_end = tokenize->start;
		check_quotes_in_loop(set, ref, &count);
		if ((count % 2) == 0 && *set->tmp_end == ref)
		{
			set->tmp_end++;
			tokenize->start = set->tmp_end;
		}
		else
			tokenize->start = set->tmp_end;
	}
	else
	{
		ret = readline_again_for_quotes(tokenize, set, ref);
		if (ret != SUCCESS)
			return (ret);
	}
	return (SUCCESS);
}

static char *store_inside_set(char **tmp_start, char **tmp_end, char ref)
{
	char *tmp;
	ptrdiff_t offset;

	(void)ref;
	if (!tmp_start || !tmp_end || tmp_start > tmp_end)
	{
		log_errors("Invalid pointers in store_inside_set", "");
		return (NULL);
	}
	offset = *tmp_end - *tmp_start;
	tmp = ft_strndup(*tmp_start, offset);
	if (!tmp)
	{
		log_errors("Failed to allocate memory in store_inside_set", "");
		return (NULL);
	}
	return (tmp);
}

int check_set(t_For_tokenize *tokenize, char ref)
{
	t_Set set;
	char *original_input;
	int ret;
	char *tokens;

	original_input = tokenize->input;
	initialize_set(tokenize->start, &set);
	while (*set.tmp_start)
	{
		check_pairs(&set.single_quote, &set.double_quote, &set.parenthesis, *set.tmp_start);
		set.tmp_start++;
	}
	set.tmp_start = tokenize->start;
	ret = call_readline_again(tokenize, &set, ref);
	if (ret != SUCCESS)
		return (ret);
	if (tokenize->input != original_input)
	{
		return (check_set(tokenize, ref));
	}
	tokens = store_inside_set(&set.tmp_start, &set.tmp_end, ref);
	if (!tokens)
		return (FAIL);
	tokenize->tokens[tokenize->token_count] = tokens;
	return (SUCCESS);
}

int handle_set(t_For_tokenize *tokenize, char ref)
{
	int result = check_set(tokenize, ref);
	if (result != SUCCESS)
	{
		all_free(&tokenize->tokens);
		log_errors("Failed to handle set token in tokenize_input", "");
		return (result);
	}
	tokenize->token_count++;
	tokenize->tokens = ft_realloc_double(tokenize->tokens, tokenize->token_count, &tokenize->buffsize);
	if (!tokenize->tokens)
		return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
	return (SUCCESS);
}
