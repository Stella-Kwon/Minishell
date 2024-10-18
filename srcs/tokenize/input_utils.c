/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:50 by skwon2           ###   ########.fr       */
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
	int			exit;

	readline_signal();
	offset = tokenize->start - tokenize->input;
	exit = read_line(&new_input);
	if (exit == FAIL)
		return (FAIL);
	if (exit == 3)
		return (3);
	init_signal();
	if (join_inputs(tokenize, new_input) == FAIL)
		return (FAIL);
	tokenize->start = tokenize->input + offset;
	add_history(tokenize->input);
	return (SUCCESS);
}

static int	check_input_loop(const char *input, int *in_single_quote,
							int *in_double_quote)
{
	int	len;
	int	i;

	i = 0;
	len = strlen(input);
	while (i < len)
	{
		if (input[i] == '\'' && !*in_double_quote)
			*in_single_quote = !*in_single_quote;
		else if (input[i] == '"' && !*in_single_quote)
			*in_double_quote = !*in_double_quote;
		if (!*in_single_quote && !*in_double_quote)
		{
			if (input[i] == ';' || input[i] == '\\')
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd("syntax error: invalid input\n", 2);
				return (FAIL);
			}
		}
		i++;
	}
	return (SUCCESS);
}

int	check_input(const char *input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	if (check_input_loop(input, &in_single_quote, &in_double_quote) == FAIL)
		return (FAIL);
	if (in_single_quote || in_double_quote)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error: unclosed quotes\n", 2);
		return (FAIL);
	}
	return (SUCCESS);
}
