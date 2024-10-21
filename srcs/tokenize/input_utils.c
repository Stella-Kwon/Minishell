/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/20 18:42:36 by skwon2           ###   ########.fr       */
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

// int check_operation_next(t_For_tokenize *tokenize)
// {
// 	char *new_input;
// 	ptrdiff_t offset;

// 	readline_signal();
// 	offset = tokenize->start - tokenize->input;
// 	new_input = readline("> ");
// 	if (!new_input)
// 		return (3);
// 	init_signal();
// 	if (join_inputs(tokenize, &new_input) == FAIL)
// 		return (FAIL);
// 	tokenize->start = tokenize->input + offset;
// 	add_history(tokenize->input);
// 	return (SUCCESS);
// }
///////////////////////////////////////////////////////////
// int operation_next_child(char **new_input)
// {

// 	while (1)
// 	{
// 		*new_input = readline("> ");
// 		if (!*new_input)
// 			exit(3);
// 		if (ft_strcmp(*new_input, *new_input) == 0)
// 			break ;
// 	}
// 	free_one((void **)new_input);

// 	exit(SUCCESS);
// }

// int check_operation_next(t_For_tokenize *tokenize)
// {
// 	char *rm_new_input;
// 	int status;
// 	pid_t pid;
// 	char *new_input;
// 	int exitcode;
// 	ptrdiff_t offset;

// 	offset = tokenize->start - tokenize->input;
// 	pid = fork();
// 	if (pid == -1)
// 		return (log_errors("Failed to fork in check_operation_next", ""));
// 	if (pid == 0)
// 	{
// 		readline_signal();
// 		return (operation_next_child(&new_input));
// 	}
// 	if (waitpid(pid, &status, 0) == -1)
// 	{
// 		exitcode = waitpid_status(status);
// 		return (exitcode);
// 	}
// 	rm_new_input = rm_quotes(new_input);
// 	if (!rm_new_input)
// 		exit(log_errors("Failed to rm_quotes in hereodc_child", ""));
// 	free_one((void **)&new_input);
// 	new_input = rm_new_input;
// 	if (join_inputs(tokenize, &new_input) == FAIL)
// 		return (FAIL);
// 	tokenize->start = tokenize->input + offset;
// 	add_history(tokenize->input);
// 	exitcode = waitpid_status(status);
// 	if (exitcode == 3)
// 		return (3);
// 	return (SUCCESS);
// }
///////////////////////////////////////////////////////////////////



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
