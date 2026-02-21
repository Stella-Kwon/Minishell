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

int check_input_set(const char *input, char ref)
{
	int i = 0;
	while (input[i])
	{
		if (input[i] == ref)
			return SUCCESS;
		i++;
	}
	return FAIL;
}

int readline_again_for_quotes(t_For_tokenize *tokenize, t_Set *set, char ref)
{
	char *result;
	char *input;
	int ret;
	char *tmp;
	char *original_input;
	ptrdiff_t offset;

	original_input = tokenize->input;
	offset = tokenize->start - original_input;
	while (set->single_quote || set->double_quote || set->parenthesis > 0)
	{
		input = read_line_safe("> ");
		if (!input || input == (char *)-1)
		{
			if (input == (char *)-1)
				return (FAIL);
			return (2);
		}
		ret = check_input_set((const char *)input, ref);
		tmp = ft_strjoin("\n", input);
		free_one((void **)&input);
		if (!tmp)
		{
			log_errors("ft_strjoin has failed in tmp:readline_again_for_quotes", "");
			return (FAIL);
		}
		result = ft_strjoin(tokenize->input, tmp);
		free_one((void **)&tmp);
		if (!result)
		{
			log_errors("ft_strjoin has failed in result:readline_again_for_quotes", "");
			return (FAIL);
		}
		free_one((void **)&tokenize->input);
		tokenize->input = result;
		if (offset >= 0)
			tokenize->start = tokenize->input + offset;
		if (isatty(STDIN_FILENO))
			add_history(tokenize->input);
		if (ret == SUCCESS)
			break;
	}
	return (SUCCESS);
}

char *rm_quotes(char *str)
{
	int i;
	int j;
	int len;
	char *tmp;
	char *result;
	char ref;

	if (!str || !*str)
		return (ft_strdup(""));

	i = 0;
	j = 0;
	ref = '\0';
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			ref = str[i];
			break;
		}
		i++;
	}
	if (!ref)
		return (ft_strdup(str));
	len = ft_strlen(str);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] != ref)
		{

			(tmp)[j] = str[i];
			// printf("tmp[%d] : %c\n", j, tmp[j]);
			j++;
			i++;
		}
		else
			i++;
	}
	(tmp)[j] = '\0';
	result = ft_strdup(tmp);
	free_one((void **)&tmp);
	return (result);
}

int readline_again_after_operator(t_For_tokenize *tokenize)
{
	char *input;
	char *tmp;
	char *result;

	input = read_line_safe("> ");
	if (!input || input == (char *)-1)
	{
		if (input == (char *)-1)
			return (FAIL);
		return (2);
	}
	tmp = ft_strjoin("\n", input);
	free_one((void **)&input);
	if (!tmp)
		return (FAIL);
	result = ft_strjoin(tokenize->input, tmp);
	free_one((void **)&tmp);
	if (!result)
		return (FAIL);

	free_one((void **)&tokenize->input);
	tokenize->input = result;
	if (isatty(STDIN_FILENO))
		add_history(tokenize->input);

	return (SUCCESS);
}

int readline_again_for_heredoc(t_For_tokenize *tokenize)
{
	char *tmp;
	int should_expand;
	const char *limiter;

	limiter = tokenize->heredoc_limiters[tokenize->heredoc_count];
	should_expand = !tokenize->heredoc_quoted[tokenize->heredoc_count];
	while (1)
	{
		tmp = read_line_safe("> ");
		if (!tmp || tmp == (char *)-1)
		{
			if (tmp == (char *)-1)
				return (FAIL);
			return 2;
		}
		if (ft_strcmp(tmp, limiter) == 0)
		{
			free_one((void **)&tmp);
			break;
		}
		if (should_expand && find_dollar_signs(&tmp, *tokenize->env, *tokenize->last_exit_code) == FAIL)
		{
			free_one((void **)&tmp);
			return (log_errors("find_dollar_signs has failed in readline_again_for_heredoc", ""));
		}
		if (tokenize->heredoc_bodies[tokenize->heredoc_count])
		{
			tokenize->heredoc_bodies[tokenize->heredoc_count] = ft_strjoin3(
				tokenize->heredoc_bodies[tokenize->heredoc_count], tmp, "\n");
			free_one((void **)&tmp);
			if (!tokenize->heredoc_bodies[tokenize->heredoc_count])
				return (log_errors("ft_strjoin3 has failed in readline_again_for_heredoc", ""));
		}
		else
		{
			tokenize->heredoc_bodies[tokenize->heredoc_count] = ft_strjoin(tmp, "\n");
			free_one((void **)&tmp);
			if (!tokenize->heredoc_bodies[tokenize->heredoc_count])
				return (log_errors("ft_strjoin has failed in readline_again_for_heredoc", ""));
		}
	}
	if (!tokenize->heredoc_bodies[tokenize->heredoc_count])
	{
		tokenize->heredoc_bodies[tokenize->heredoc_count] = ft_strdup("");
		if (!tokenize->heredoc_bodies[tokenize->heredoc_count])
			return (log_errors("Failed to allocate empty heredoc body", ""));
	}
	return (SUCCESS);
}