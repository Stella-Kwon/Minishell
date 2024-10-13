/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:02 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/12 21:05:48 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t get_str_len(char **str_arr)
{
	size_t len;
	size_t i;

	len = 0;
	i = 0;
	while (str_arr[i] != NULL)
	{
		len++;
		i++;
	}
	return (len);
}

int copy_envp(char **dest, char **src, size_t len)
{
	size_t i;

	i = 0;
	while (i < len)
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			perror("cd: strdup failed");
			delete_str_array(&src);
			return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

void print_strs(char **str_arr)
{
	size_t i;

	i = 0;
	while (str_arr[i] != NULL)
	{
		ft_putstr_fd(str_arr[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

void	sort_envp(size_t len, char **envp)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j + 1 < len)
		{
			if (ft_strncmp(envp[j], envp[j + 1],ft_strlen(envp[j] + 1)) > 0)
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}
