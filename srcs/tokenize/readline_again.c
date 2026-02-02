/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:31:30 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int read_prompt_line(char **line)
{
	int was_interrupt;
	int exitnum = SUCCESS;

	if (isatty(STDIN_FILENO))
		signal_set_rl();
	*line = read_line_safe("> ");
	if (!*line || *line == (char *)-1)
	{
		was_interrupt = (*line == (char *)-1);

		if (was_interrupt)
			exitnum = 3;
		exitnum = 2;
	}
	if (isatty(STDIN_FILENO))
		signal_setup();
	return (exitnum);
}

void update_quotes_and_depth(int *single_quote, int *double_quote,
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

void check_set_iterate(t_Set *set, char **new_start)
{
	while (**new_start)
	{
		update_quotes_and_depth(&set->single_quote,
								&set->double_quote, &set->depth, **new_start);
		if (ft_isspace(**new_start))
		{
		 	if(!set->single_quote && !set->double_quote && set->depth == 0)
				break;
		}
		(*new_start)++;
	}
}

static int prepare_new_start(char **input, char **new_start, char **result)
{
	char *new_input;
	char *tmp;
	int ret;

	ret = read_prompt_line(&new_input);
	if (ret != SUCCESS)
		return (ret);
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

int readline_again(t_For_tokenize *tokenize, t_Set *set)
{
	char *new_start;
	char *result;
	ptrdiff_t offset;
	int ret;

	while (set->single_quote || set->double_quote || set->depth > 0)
	{
		offset = tokenize->start - tokenize->input;
		ret = prepare_new_start(&tokenize->input, &new_start, &result);
		if (ret != SUCCESS)
			return (ret);
		check_set_iterate(set, &new_start);
		free_one((void **)&tokenize->input);
		tokenize->input = result;
		tokenize->start = tokenize->input + offset;
		set->tmp_start = tokenize->start;
		set->tmp_end = new_start;
		if (isatty(STDIN_FILENO))
			add_history(tokenize->input);
	}
	return (SUCCESS);
}
