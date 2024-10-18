/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 11:31:30 by sukwon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	prepare_new_start(char **input, char **new_start, char **result)
{
	char		*new_input;
	char		*tmp;

	new_input = readline("> ");
	if (!new_input)
		return (log_errors("new_input readline(>) failed", ""));
	tmp = ft_strjoin("\n", new_input);
	free(new_input);
	if (!tmp)
		return (log_errors("Fail to add new line before add readline", ""));
	*result = ft_strjoin(*input, tmp);
	free(tmp);
	if (!*result)
		return (log_errors("Fail to strjoin in \"readline_again\"", ""));
	*new_start = *result + ft_strlen(*input);
	return (SUCCESS);
}

void	update_quotes_and_depth(int *single_quote, int *double_quote, \
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

void	check_set_iterate(t_Set *set, char **new_start)
{
	while (**new_start)
	{
		update_quotes_and_depth(&set->single_quote, \
								&set->double_quote, &set->depth, **new_start);
		if (!set->single_quote && !set->double_quote && set->depth == 0)
			break ;
		(*new_start)++;
	}
}

int	readline_again(t_For_tokenize *tokenize, t_Set *set)
{
	char		*new_start;
	char		*result;
	ptrdiff_t	offset;

	while (set->single_quote || set->double_quote || set->depth > 0)
	{
		offset = tokenize->start - tokenize->input;
		if (prepare_new_start(&tokenize->input, &new_start, &result) == FAIL)
			return (FAIL);
		check_set_iterate(set, &new_start);
		free_one((void **)&tokenize->input);
		tokenize->input = result;
		tokenize->start = tokenize->input + offset;
		set->tmp_start = tokenize->start;
		set->tmp_end = new_start;
		add_history(tokenize->input);
	}
	return (SUCCESS);
}
