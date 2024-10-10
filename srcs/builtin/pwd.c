/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:35 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 00:33:36 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	pwd(char **args, char ***envp_ptr)
// {
// 	char	*buf;

// 	(void)args;
// 	(void)envp_ptr;
// 	buf = ft_calloc(1, MAXPATHLEN);
// 	if (buf == NULL)
// 	{
// 		perror("Error: malloc failed");
// 		return (FAIL);
// 	}
// 	if (getcwd(buf, MAXPATHLEN) == NULL)
// 	{
// 		perror("Error: getcwd failed");
// 		free(buf);
// 		return (FAIL);
// 	}
// 	ft_putstr_fd(buf, STDOUT_FILENO);
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	free(buf);
// 	return (SUCCESS);
// }


int pwd(t_Command *command)
{
    char *buf;

    buf = ft_calloc(1, MAXPATHLEN);
    if (buf == NULL)
    {
        perror("Error: malloc failed");
        command->exitcode = FAIL;
        return (command->exitcode);
    }
    if (getcwd(buf, MAXPATHLEN) == NULL)
    {
        perror("Error: getcwd failed");
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
