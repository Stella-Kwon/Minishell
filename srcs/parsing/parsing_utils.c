/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:52:16 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 00:14:46 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char **tokens)
{
	if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 || \
		ft_strcmp(*tokens, "||") == 0 || ft_strcmp(*tokens, "|") == 0 || \
		ft_strcmp(*tokens, "(") == 0)
		return (1);
	return (0);
}

int	get_direction_type(char *token)
{
	if (ft_strcmp(token, ">>") == 0)
		return (REDIRECT_APPEND);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUTPUT);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_INPUT);
	return (INVALID);
}

