/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:35:17 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/17 13:39:19 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_herestring(t_Redirection **redirect, char *string)
{
	char	*str;

	str = ft_strdup(string);
	if (!str)
		return (log_errors("Failed malloc in set_herestring", ""));
	(*redirect)->herestring_str = str;
	(*redirect)->last_stdin_type = 3;
	return (SUCCESS);
}

int	herestring_action(char ***args, t_Redirection **redirect)
{
	(*args)++;
	if (set_herestring(redirect, **args) == FAIL)
		return (FAIL);
	(*args)++;
	return (SUCCESS);
}
