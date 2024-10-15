/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/13 22:54:50 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_operator(char **tokens)
{
	if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 ||
		ft_strcmp(*tokens, "||") == 0 || ft_strcmp(*tokens, "|") == 0 ||
		ft_strcmp(*tokens, "(") == 0)
		return (1);
	return (0);
}

int get_direction_type(char *token)
{
	if (ft_strcmp(token, ">>") == 0)
		return (REDIRECT_APPEND);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUTPUT);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_INPUT);
	return (INVALID);
}

int put_last_open_infile(t_Redirection **redirect, char ***args, char **filename)
{
	char	*start;

	start = **args;
	while (**args && is_redirection(**args) == FALSE)
	{
		if (access(start, F_OK) != 0)
			break;
		else
			(*args)++;
		if (access(**args, F_OK) != 0)
		{
			(*args)--;
			break;
		}
		(*args)++;
	}
	if (rm_quote_filename(redirect, args, filename) != SUCCESS)
		return (log_errors("Failed in rm_quote_filename in set_redirection", ""));
	return (SUCCESS);
}