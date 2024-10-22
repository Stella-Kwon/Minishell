/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_next.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 22:30:20 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int join_inputs(char **tokenize_input, char **new_input)
{
    char *tmp;

    tmp = ft_strjoin(" ", *new_input);
    free_one((void **)new_input);
    if (!tmp)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }
    *new_input = ft_strjoin(*tokenize_input, tmp);
	free_one((void **)tokenize_input);
    free_one((void **)&tmp);
    if (!*new_input)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }
    *tokenize_input = *new_input;
    return (SUCCESS);
}

int check_operation_next(t_For_tokenize *tokenize)
{
	char *new_input;
	ptrdiff_t offset;
	char *cpy_input;
	char *cpy_new_input;

	offset = tokenize->start - tokenize->input;
	new_input = readline("> ");
	if (!new_input)
		return (3);
	cpy_input = ft_strdup(tokenize->input);
	if (!cpy_input)
		return (log_errors("Failed to strdup:tok from op_next_parents", ""));
	cpy_new_input = ft_strdup(new_input);
	if (!cpy_new_input)
		return (log_errors("Failed to strdup:new from op_next_parents", ""));
	if (join_inputs(&tokenize->input, &new_input) == FAIL)
		return (FAIL);
	add_history(tokenize->input);
	if (rm_quote_buffer(tokenize, &cpy_new_input, &cpy_input) == FAIL)
		return (FAIL);
	tokenize->start = tokenize->input + offset;
	return (SUCCESS);
}
