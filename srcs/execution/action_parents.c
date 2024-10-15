/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:05 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 21:59:17 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	action_parents(t_Redirection **redir, t_Pipeline **pipeline, t_Command **cmd)
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
