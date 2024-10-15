/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:41 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/12 21:06:10 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_env_var(t_Command *command, char **new)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	**var_name;

	len = get_str_len(*(command->env));
	i = 0;
	j = 0;
	while (i < len)
	{
		var_name = ft_split((*(command->env))[i], '=');
		if (ft_strncmp(var_name[0], command->args[1], \
			ft_strlen(var_name[0]) + 1) != 0)
		{
			new[j] = (*(command->env))[i];
			j++;
		}
		delete_str_array(&var_name);
		i++;
	}
	new[j] = NULL;
}

int	unset(t_Command *command)
{
	char	**new_envp;
	size_t	len;

	if (command->args[1] != NULL && command->args[1][0] == '-')
	{
		ft_putstr_fd("unset : invalid option", STDERR_FILENO);
		ft_putstr_fd(command->args[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	if (command->args[1] != NULL)
	{
		len = get_str_len(*(command->env));
		new_envp = ft_calloc(len + 1, sizeof(*new_envp));
		if (new_envp == NULL)
		{
			perror("unset: malloc failed");
			command->exitcode = FAIL;
			return (command->exitcode);
		}
		remove_env_var(command, new_envp);
		delete_str_array(command->env);
		*(command->env) = new_envp;
	}
	command->exitcode = SUCCESS;
	return (command->exitcode);
}
