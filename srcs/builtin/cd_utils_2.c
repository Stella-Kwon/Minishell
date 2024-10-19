/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 04:34:46 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 04:34:48 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	currdir_to_old_pwd(t_Command *command)
{
	char	*path;
	char	*tmp;

	path = get_current_dir("OLDPWD=");
	if (!path)
		return ;
	tmp = command->args[1];
	command->args[1] = path;
	export(command);
	command->args[1] = tmp;
	free(path);
}

char	*get_current_dir(char *prefix)
{
	char	*path;
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (!buf)
	{
		log_errors("Failed calloc in cd utils2", "");
		return (NULL);
	}
	if (!getcwd(buf, MAXPATHLEN))
	{
		log_errors("Failed getcwd in cd utils2", "");
		free(buf);
		return (NULL);
	}
	path = ft_strjoin(prefix, buf);
	free(buf);
	if (!path)
	{
		log_errors("Failed strjoin in cd utils2", "");
		return (NULL);
	}
	return (path);
}
