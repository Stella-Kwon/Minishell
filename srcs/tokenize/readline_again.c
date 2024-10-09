/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/08 22:40:10 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdio.h> // printf를 사용하기 위해 필요합니다.

// static int prepare_new_start(char **input, char **new_start, char **result)
// {
// 	char *new_input;
// 	char *tmp;

// 	new_input = readline("> ");
// 	if (!new_input)
// 	{
// 		printf("Error: new_input readline(>) failed\n");
// 		return log_errors("new_input readline(>) failed", "");
// 	}
// 	printf("New input received: %s\n", new_input);

// 	tmp = ft_strjoin("\n", new_input);
// 	free_one((void **)&new_input);
// 	if (new_input == NULL)
// 		printf("new_input is null");
// 	if (!tmp)
// 	{
// 		printf("Error: Failed to add new line before add readline\n");
// 		return log_errors("Fail to add new line before add readline", "");
// 	}
// 	printf("Temporary string after strjoin: %s\n", tmp);

// 	*result = ft_strjoin(*input, tmp);
// 	free_one((void **)&tmp);
// 	if (tmp == NULL)
// 		printf("tmp is null");
// 	if (!*result)
// 	{
// 		printf("Error: Fail to strjoin in \"readline_again\"\n");
// 		return log_errors("Fail to strjoin in \"readline_again\"", "");
// 	}
// 	*new_start = *result + ft_strlen(*input);
// 	printf("New start pointer set successfully.\n");
// 	return SUCCESS;
// }

static int	prepare_new_start(char **input, char **new_start, char **result)
{
	char		*new_input;
	char		*tmp;

	new_input = readline("> ");
	if (!new_input)
		return (log_errors("new_input readline(>) failed", ""));
	tmp = ft_strjoin("\n", new_input);
	free_one((void**)&new_input);
	if (!tmp)
		return (log_errors("Fail to add new line before add readline", ""));
	*result = ft_strjoin(*input, tmp);
	free_one((void**)&tmp);
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

	while (set->single_quote || set->double_quote || set->depth > 0)
	{
		offset = tokenize->start - tokenize->input;
		if (prepare_new_start(&tokenize->input, &new_start, &result) == FAIL)
			return (FAIL);
		while (*new_start)
		{
			update_quotes_and_depth(&set->single_quote,
									&set->double_quote, &set->depth, *new_start);
			if (!set->single_quote && !set->double_quote && set->depth == 0)
				break;
			new_start++;
		}
		// printf("hod\n");
		// if (tokenize->input)
		// 	printf("tokenize->input is %s\n", tokenize->input);
		free_one((void**)&tokenize->input);
		// if (tokenize->input == NULL)
		// 	printf("tokenize->input is null\n");
		tokenize->input = result;
		tokenize->start = tokenize->input + offset;
		set->tmp_start = tokenize->start;
		set->tmp_end = new_start;
		add_history(tokenize->input);
	}
	return (SUCCESS);
}
