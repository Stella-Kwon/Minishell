/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:41 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 03:17:17 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	fill_new_env(char **new, t_Command *command, size_t i, size_t *j)
{
	size_t	equal_pos;
	char	*var_name;

	equal_pos = 0;
	while ((*(command->env))[i][equal_pos] && \
			(*(command->env))[i][equal_pos] != '=')
		equal_pos++;
	var_name = ft_strndup((*(command->env))[i], equal_pos);
	if (!var_name)
		return (log_errors("Failed strndup in remove_env_var", ""));
	if (ft_strcmp(var_name, command->args[1]) != 0)
	{
		new[*j] = ft_strdup((*(command->env))[i]);
		if (!(new[*j]))
		{
			free(var_name);
			return (log_errors("Failed strndup in remove_env_var", ""));
		}
		(*j)++;
	}
	free(var_name);
	return (SUCCESS);
}

static int	remove_env_var(t_Command *command, char **new)
{
	size_t	i;
	size_t	j;
	size_t	len;

	len = get_str_len(*(command->env));
	i = 0;
	j = 0;
	while (i < len)
	{
		if (fill_new_env(new, command, i, &j) == FAIL)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

static int	process_unset_args(t_Command *command)
{
	size_t	len;
	char	**new_envp;

	len = get_str_len(*(command->env));
	new_envp = ft_calloc(len + 1, sizeof(*new_envp));
	if (new_envp == NULL)
	{
		log_errors("Failed calloc in unset, process unset", "");
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	if (remove_env_var(command, new_envp) == FAIL)
	{
		delete_str_array(&new_envp);
		return (FAIL);
	}
	delete_str_array(command->env);
	*(command->env) = new_envp;
	return (SUCCESS);
}

int	unset(t_Command *command)
{
	if (command->args[1] != NULL && command->args[1][0] == '-')
	{
		ft_putstr_fd("unset: invalid option", STDERR_FILENO);
		ft_putstr_fd(command->args[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	if (command->args[1] != NULL)
	{
		if (process_unset_args(command) == FAIL)
			return (command->exitcode);
	}
	command->exitcode = SUCCESS;
	return (command->exitcode);
}
