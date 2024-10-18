/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 05:04:17 by skwon2           ###   ########.fr       */
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
			handle_258_exitcode_print("`||'");
		else
			handle_258_exitcode_print("`|'");
	}
	else if (*tokenize->start == '&')
	{
		if (*(tokenize->start + 1) == '&')
			handle_258_exitcode_print("`&&'");
		else
			handle_258_exitcode_print("`&'");
	}
	else
		return (SUCCESS);
	return (FAIL);
}

static int	join_inputs(t_For_tokenize *tokenize, char *new_input)
{
	char		*tmp;

	tmp = ft_strjoin(" ", new_input);
	free(new_input);
	if (!tmp)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	new_input = ft_strjoin(tokenize->input, tmp);
	free(tmp);
	if (!new_input)
	{
		log_errors("Failed to join inputs", "");
		return (FAIL);
	}
	free(tokenize->input);
	tokenize->input = new_input;
	return (SUCCESS);
}

int	check_operation_next(t_For_tokenize *tokenize)
{
	char		*new_input;
	ptrdiff_t	offset;

	init_signal();
	offset = tokenize->start - tokenize->input;
	new_input = readline("> ");
	if (!new_input)
	{
		ft_putstr_fd("syntax error: unexpected end of file\n", 2);
		return (2);
	}
	if (g_received_signal == 1)
	{
		free_one((void**)&new_input);
		return (-1);
	}
	if (join_inputs(tokenize, new_input) == FAIL)
		return (FAIL);
	tokenize->start = tokenize->input + offset;
	add_history(tokenize->input);
	return (SUCCESS);
}
