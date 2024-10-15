/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:21 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 04:44:18 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_valid(char *var_name)
{
	size_t	i;

	i = 1;
	if (!var_name || var_name[0] == '=' || !(ft_isalpha(var_name[0])))
		return (false);
	while (var_name[i] != '\0' && var_name[i] != '=')
	{
		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

static int	export_without_args(t_Command *command)
{
	if (command->env == NULL)
		return (cmd_error(&command, "env not initialized\n", 1));
	if (print_sorted_envp(*(command->env)) == SUCCESS)
	{
		command->exitcode = 0;
		return (SUCCESS);
	}
	command->exitcode = 1;
	return (FAIL);
}

static int	invalid_identifier_error(t_Command *command, char **var_name, \
									char **value, char **tmp)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(*tmp, STDERR_FILENO);
	ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
	free(*tmp);
	free(*var_name);
	free(*value);
	command->exitcode = 1;
	return (FAIL);
}

static int	get_name_and_value(t_Command *command, char **var_name, \
								char **value, int i)
{
	char	*tmp;
	size_t	equal_pos;

	tmp = ft_strdup(command->args[i]);
	if (!tmp)
	{
		log_errors("export: strdup failed", "");
		command->exitcode = 1;
		return (FAIL);
	}
	equal_pos = 0;
	while (tmp[equal_pos] && tmp[equal_pos] != '=')
		equal_pos++;
	*var_name = ft_substr(tmp, 0, equal_pos);
	if (tmp[equal_pos] == '=')
		*value = ft_substr(tmp, equal_pos + 1, ft_strlen(tmp) - equal_pos - 1);
	else
		*value = NULL;
	if (!(*var_name) || !is_valid(*var_name) || \
		(tmp[equal_pos] == '=' && !tmp[equal_pos + 1] && !(*value)))
		return (invalid_identifier_error(command, var_name, value, &tmp));
	free(tmp);
	return (SUCCESS);
}

int	export(t_Command *command)
{
	char	*var_name;
	char	*value;
	size_t	i;

	var_name = NULL;
	value = NULL;
	if (command->args[1] == NULL)
		return (export_without_args(command));
	i = 1;
	while (command->args[i] != NULL)
	{
		if (get_name_and_value(command, &var_name, &value, i) == FAIL)
			return (FAIL);
		add_env_item(command, var_name, value);
		free(var_name);
		free(value);
		i++;
	}
	command->exitcode = 0;
	return (SUCCESS);
}
