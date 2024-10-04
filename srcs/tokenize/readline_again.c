/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:01:05 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	readline_again(char **input, t_Set *set, char **start)
{
	char		*new_input;
	char		*new_start;
	char		*result;
	char		*tmp;
	ptrdiff_t	offset;
	char		*st;
	char		*in;

	while (set->single_quote || set->double_quote || set->depth > 0)
	{
		st = *start;
		in = *input;
		offset = st - in;
		new_input = readline("> ");
		if (!new_input)
			return (log_errors("new_input readline(>) failed", ""));
		tmp = ft_strjoin("\n", new_input);
		free(new_input);
		if (!tmp)
			return (log_errors("Fail to add new line before add readline", ""));
		result = ft_strjoin(*input, tmp);
		free(tmp);
		if (!result)
			return (log_errors("Fail to strjoin in \"readline_again\"", ""));
		new_start = result + ft_strlen(*input);
		while (*new_start)
		{
			update_quotes_and_depth(&set->single_quote,
				&set->double_quote, &set->depth, *new_start);
			if (!set->single_quote && !set->double_quote && set->depth == 0)
				break ;
			new_start++;
		}
		free(*input);
		*input = result;
		*start = *input + offset;
		set->tmp_start = *start;
		set->tmp_end = new_start;
		add_history(*input);
	}
	return (SUCCESS);
}
