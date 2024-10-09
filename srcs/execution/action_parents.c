/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:05 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 17:17:21 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	action_parents(t_Redirection **redir, t_Pipeline **pipeline)
{
	int	wstatus;
	int	exitcode;

	exitcode = 0;
	wstatus = -2;
	if (waitpid((*pipeline)->pid, &wstatus, 0) == -1)
		return (log_errors("waitpid failed", ""));
	if ((*redir)->outfile >= 0)
		close((*redir)->outfile);
	if ((*redir)->infile >= 0)
		close((*redir)->infile);
	exitcode = waitpid_status(wstatus);
	return (exitcode);
}
