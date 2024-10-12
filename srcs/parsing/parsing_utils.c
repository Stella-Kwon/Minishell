/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/12 00:57:58 by suminkwon        ###   ########.fr       */
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
	char **tmp_args;
	int i;

	i = 0;
	tmp_args = *args;
	while (tmp_args[i])
	{
		// printf("tmp_args[%d] : %s", i, tmp_args[i]);
		if (access(tmp_args[i], F_OK) != 0)
			break;
		i++;
	}
	if (rm_quote_filename(redirect, args) != SUCCESS)
		return (log_errors("Failed in rm_quote_filename in set_redirection", ""));
	*filename = (*redirect)->filename;
	return (SUCCESS);
}