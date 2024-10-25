/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:35 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/24 18:45:54 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pwd(t_Command *command)
{
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (!buf)
	{
		log_errors("Failed calloc in pwd", "");
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	if (!getcwd(buf, MAXPATHLEN))
	{
		free(buf);
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(buf);
	command->exitcode = SUCCESS;
	return (command->exitcode);
}
