/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:09 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/16 10:40:08 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env(t_Command *command)
{
	if (!command->cmd)
	{
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	if (command->args[1] != NULL && command->args[1][0] == '-')
	{
		ft_putstr_fd("minishell: env: invalid argument ", STDERR_FILENO);
		ft_putstr_fd(command->args[1], STDERR_FILENO);
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	print_strs(*(command->env));
	command->exitcode = SUCCESS;
	return (command->exitcode);
}

void	delete_str_array(char ***str_arr)
{
	size_t	i;

	i = 0;
	if (str_arr == NULL || *str_arr == NULL)
		return ;
	while ((*str_arr)[i] != NULL)
	{
		(*str_arr)[i] = NULL;
		i++;
	}
	*str_arr = NULL;
}

int	append_to_env(char *str, char ***envp_ptr)
{
	size_t	env_len;
	char	**new_envp;

	env_len = get_str_len(*envp_ptr);
	new_envp = ft_calloc(env_len + 2, sizeof(*new_envp));
	if (!new_envp)
	{
		log_errors("Failed calloc in append_to_env", "");
		return (FAIL);
	}
	copy_envp(new_envp, *envp_ptr, env_len);
	new_envp[env_len] = ft_strdup(str);
	if (!new_envp[env_len])
	{
		log_errors("Failed strdup in append_to_env", "");
		delete_str_array(&new_envp);
		return (FAIL);
	}
	new_envp[env_len + 1] = NULL;
	delete_str_array(envp_ptr);
	*envp_ptr = new_envp;
	return (SUCCESS);
}

char	*get_env_value(const char *key, char **envp)
{
	size_t	i;
	size_t	k_len;

	if (key == NULL || envp == NULL)
		return (NULL);
	k_len = ft_strlen(key);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, k_len) == 0 && envp[i][k_len] == '=')
			return (ft_strchr(envp[i], '=') + 1);
		i++;
	}
	return (NULL);
}
