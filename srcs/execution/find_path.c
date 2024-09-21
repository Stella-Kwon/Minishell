/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:26:28 by skwon2            #+#    #+#             */
/*   Updated: 2024/09/20 21:22:25 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_path(char **env)
{
	char	*path;
	int		i;
	int		j;

	i = 0;
	path = NULL;
	while (env[i])
	{
		j = 0;
		while (env[i][j])
		{
			if (env[i][0] == 'P' && env[i][1] == 'A' &&
				env[i][2] == 'T' && env[i][3] == 'H')
			{
				path = env[i];
				path = ft_strdup(path + 5);
				if (!path)
					log_errors("Failed to ft_strdup in find_path");
				return (path);
			}
			j++;
		}
		i++;
	}
	return (path);
}

char	**split_path(char **path)
{
	t_path	s_p;

	if (!(*path))
		return (NULL);
	s_p.split_path = ft_split_add_slush(*path, ':');
	if (!s_p.split_path)
	{
		log_errors("find_right_path ft_split_add_slush() error : NULL");
		return (NULL);
	}
	return (s_p.split_path);
}

int	precheck(char ***split_cmd, char **tmp)
{
	if (access(*split_cmd[0], X_OK) == 0)
	{
		*tmp = ft_strdup(*split_cmd[0]);
		if (!(*tmp))
		{
			log_errors("Failed to ft_strdup in precheck");
			return (FAIL);
		}
		return (SUCCESS);
	}
	return (2);
}

char	*strjoin_path(t_path *s_p, char ***split_cmd)
{
	s_p->i = 0;
	while (s_p->split_path[s_p->i])
	{
		s_p->tmp = ft_strjoin(s_p->split_path[s_p->i], *split_cmd[0]);
		if (!s_p->tmp)
		{
			log_errors("Failed to ft_strjoin in strjoin_path");
			return (NULL);
		}
		if (access(s_p->tmp, X_OK) == 0)
		{
			all_free(&s_p->split_path);
			return (s_p->tmp);
		}
		s_p->i++;
		free_one(&s_p->tmp);
	}
	return (NULL);
}

char	*find_right_path(char *argv, char **path, char ***split_cmd)
{
	t_path	s_p;
	int		res;

	res = precheck(split_cmd, &s_p.tmp);
	if (res == FAIL)
		return (NULL);
	else if (res == SUCCESS)
		return (s_p.tmp);
	s_p.split_path = split_path(path);
	if (!s_p.split_path)
		return (NULL);
	if (argv[0] != '/' && access(*split_cmd[0], X_OK) == -1)
	{
		s_p.tmp = strjoin_path(&s_p, split_cmd);
		if (!s_p.tmp)
			return (NULL);
		return (s_p.tmp);
	}
	if (s_p.split_path)
		all_free(&s_p.split_path);
	return (NULL);
}
