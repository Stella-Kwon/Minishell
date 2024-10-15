/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   waitpid_status.c								   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: suminkwon <suminkwon@student.42.fr>		+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/09/19 16:36:33 by suminkwon		 #+#	#+#			 */
/*   Updated: 2024/10/01 23:02:11 by suminkwon		###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	waitpid_status(int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 0)
			return (SUCCESS);
		else
			return (WEXITSTATUS(wstatus));
	}
	else if (WIFSIGNALED(wstatus))
	{
		// log_errors("WAIT_STATUS : Child process terminated due to signal", "");
		if (g_received_signal == 130)
			return (130);
		else if (g_received_signal == 131)
			return (131);
		else
			return (WTERMSIG(wstatus));
	}
	else if (WIFSTOPPED(wstatus))
	{
		// log_errors("WAIT_STATUS : Child process was stopped by signal", "");
		return (WSTOPSIG(wstatus));
	}
	// ft_putstr_fd("Unexpected termination status\n", 2);
	return (FAIL);
}
