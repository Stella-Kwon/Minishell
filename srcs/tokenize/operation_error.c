/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:00:05 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_first_input(char **start)
{
	while (ft_isspace(*(*start)))
		(*start)++;
	if (**start == '|')
	{
		if (*(*start + 1) == '|')
			handle_258_exitcode_print("'||'");
		else
			handle_258_exitcode_print("'|'");
	}
	else if (**start == '&')
	{
		if (*(*start + 1) == '&')
			handle_258_exitcode_print("'&&'");
		else
			handle_258_exitcode_print("'&'");
	}
	else
		return (SUCCESS);
	return (FAIL);
}

static int	join_inputs(char **input, char *new_input)
{
	char		*tmp;

	tmp = ft_strjoin(" ", new_input);
	free(new_input);
	if (!tmp)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	new_input = ft_strjoin(*input, tmp);
	free(tmp);
	if (!new_input)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	free(*input);
	*input = new_input;
	return (SUCCESS);
}

int	check_operation_next(char **input, char **start, int flag)
{
	char		*new_input;
	ptrdiff_t	offset;
	char		*st;
	char		*in;

	st = *start;
	in = *input;
	offset = st - in;
	new_input = readline("> ");
	if (!new_input)
	{
		log_errors("Failed to readline for additional input", "");
		return (FAIL);
	}
	if (flag)
		add_history(new_input);
	if (join_inputs(input, new_input) == FAIL)
		return (FAIL);
	*start = *input + offset;
	add_history(*input);
	return (SUCCESS);
}
