/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/08 01:17:02 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_first_input(t_For_tokenize *tokenize)
{
	while (ft_isspace(*tokenize->start))
		tokenize->start++;
	if (*tokenize->start == '|')
	{
		if (*(tokenize->start + 1) == '|')
			handle_258_exitcode_print("'||'");
		else
			handle_258_exitcode_print("'|'");
	}
	else if (*tokenize->start == '&')
	{
		if (*(tokenize->start + 1) == '&')
			handle_258_exitcode_print("'&&'");
		else
			handle_258_exitcode_print("'&'");
	}
	else
		return (SUCCESS);
	return (FAIL);
}

static int	join_inputs(t_For_tokenize *tokenize, char *new_input)
{
	char	*tmp;

	free_one((void **)&tokenize->input);
	tmp = ft_strjoin(" ", new_input);
	if (!tmp)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	tokenize->input = ft_strjoin(tokenize->input, tmp);
	free_one((void **)&tmp);
	if (!tokenize->input)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	return (SUCCESS);
}

int	check_operation_next(t_For_tokenize *tokenize, int flag)
{
	char		*new_input;
	ptrdiff_t	offset;

	offset = tokenize->start - tokenize->input;
	new_input = readline("> ");
	if (!new_input)
	{
		log_errors("Failed to readline for additional input", "");
		return (FAIL);
	}
	if (flag)
		add_history(new_input);
	if (join_inputs(tokenize, new_input) == FAIL)
		return (FAIL);
	*tokenize->start = *tokenize->input + offset;
	add_history(tokenize->input);
	return (SUCCESS);
}
