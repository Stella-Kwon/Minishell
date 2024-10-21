/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:05 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 01:31:44 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	action_parents(t_Redirection **redir, t_Pipeline **pipeline, \
					t_Command **cmd)
{
	int	wstatus;

	wstatus = -2;
	if (waitpid((*pipeline)->pid, &wstatus, 0) == -1)
	{
		(*cmd)->exitcode = waitpid_status(wstatus);
		return ((*cmd)->exitcode);
	}
	if ((*redir)->outfile >= 0)
		close((*redir)->outfile);
	if ((*redir)->infile >= 0)
		close((*redir)->infile);
	(*cmd)->exitcode = waitpid_status(wstatus);
	return ((*cmd)->exitcode);
}
