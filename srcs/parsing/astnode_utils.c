/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astnode_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 04:29:27 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:21:51 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redirection(char *token)
{
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0 || \
		ft_strcmp(token, "<<<") == 0 || ft_strcmp(token, ">") == 0 || \
		ft_strcmp(token, ">>") == 0)
		return (TRUE);
	return (FALSE);
}

int	remove_args_after_redirection(char ***args)
{
	t_rm_args	rm;

	rm.buffersize = BUFFER_SIZE;
	rm.i = 0;
	rm.origin_i = 0;
	rm.new_args = ft_calloc(rm.buffersize, sizeof(char *));
	if (!rm.new_args)
		return (log_errors("Failed to malloc new_args in \
				remove_args function", ""));
	if (restore_new_args(args, &rm) != SUCCESS)
		return (FAIL);
	if (*args)
	{
		all_free(args);
	}
	*args = rm.new_args;
	return (SUCCESS);
}
