/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 04:34:39 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*store_words(char **start)
{
	char	*word_start;

	word_start = *start;
	while (**start && !ft_isspace(**start) && **start != '(' && \
			**start != ')' && **start != '|' && **start != '&' && \
			**start != '>' && **start != '<')
	{
		(*start)++;
	}
	return (ft_strndup(word_start, *start - word_start));
}

int	store_str(t_For_tokenize *tokenize, int *buffsize)
{
	if (!ft_isspace(*tokenize->start) && *tokenize->start != '\0')
	{
		tokenize->tokens[tokenize->token_count] = \
		store_words(&tokenize->start);
		if (!tokenize->tokens[tokenize->token_count])
		{
			all_free(&tokenize->tokens);
			return (log_errors("Failed to store word", ""));
		}
		tokenize->token_count++;
		tokenize->tokens = ft_realloc_double(tokenize->tokens, \
		tokenize->token_count, buffsize);
		if (!tokenize->tokens)
			return (log_errors("Failed to \"reallocate\" \
			memory for tokens", ""));
	}
	return (SUCCESS);
}

int	is_special_character(char c)
{
	if (c == '\\' || c == ';' || c == '^' || c == '#' || \
		c == '@' || c == '!' || c == '*' || c == '%' || c == '(' || c == ')')
		return (TRUE);
	return (FALSE);
}

static int	check_input_loop(const char *input, int *in_single_quote, \
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
