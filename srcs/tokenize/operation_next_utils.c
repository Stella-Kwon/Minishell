/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_next_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 03:04:11 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 20:59:25 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	rm_quote_buffer(t_For_tokenize *tokenize, char **buffer, char **cp_input)
{
	char	*rm_quote_buffer;

	rm_quote_buffer = rm_quotes(*buffer, 0);
	if (!rm_quote_buffer)
		return (log_errors("Failed to rm_quotes in rm_quote_buffer", ""));
	free_one((void **)buffer);
	if (join_inputs(cp_input, &rm_quote_buffer) == FAIL)
		return (FAIL);
	if (!*cp_input)
		return (log_errors("Failed to join_inputs in rm_quote_buffer", ""));
	if (tokenize->input)
		free_one((void **)&tokenize->input);
	tokenize->input = *cp_input;
	return (SUCCESS);
}
