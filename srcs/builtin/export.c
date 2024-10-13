/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:21 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 16:04:38 by hlee-sun         ###   ########.fr       */
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

static int	print_env_var(char *env_var)
{
	size_t	i;

	i = 0;
	if (!env_var)
		return (FAIL);
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (env_var[i] != '\0')
	{
		ft_putchar_fd(env_var[i], STDOUT_FILENO);
		if (env_var[i] == '=')
			ft_putstr_fd("\"", STDOUT_FILENO);
		i++;
	}
	if (ft_strchr(env_var, '=') != NULL)
		ft_putstr_fd("\"", STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}

static int	print_sorted_envp(char **envp)
{
	size_t	i;
	size_t	len;
	char	**sorted_envp;

	if (!envp)
		return (FAIL);
	len = get_str_len(envp);
	sorted_envp = ft_calloc(len + 1, sizeof(*sorted_envp));
	if (!sorted_envp)
	{
		perror("Error: malloc failed");
		return (FAIL);
	}
	if (copy_envp(sorted_envp, envp, len) == FAIL)
		return (FAIL);
	sort_envp(len, sorted_envp);
	i = 0;
	while (i < len)
	{
		if (sorted_envp[i][0] != '_' && sorted_envp[i][1] != '=')
		{
			if (print_env_var(sorted_envp[i]) == FAIL)
			{
				delete_str_array(&sorted_envp);
				return (FAIL);
			}
		}
		i++;
	}
	delete_str_array(&sorted_envp);
	return (SUCCESS);
}

// int	export(t_Command *command)
// {
// 	char	*var_name;
// 	char	*value;
// 	char	*tmp;
// 	size_t	i;

// 	if (command->args[1] == NULL)
// 	{
// 		if (command->env == NULL)
// 			return (cmd_error(&command, "env not initialized\n", 1));
// 		if (print_sorted_envp(*(command->env)) == SUCCESS)
// 		{
// 			command->exitcode = 0;
// 			return (SUCCESS);
// 		}
// 		command->exitcode = 1;
// 		return (FAIL);
// 	}
// 	i = 0;
// 	tmp = ft_strdup(command->args[1]);
// 	if (!tmp)
// 	{
// 		perror("minishell: strdup failed");
// 		command->exitcode = FAIL;
// 		return (command->exitcode);
// 	}
// 	while (tmp[i] && tmp[i] != '=')
// 		i++;
// 	var_name = ft_substr(tmp, 0, i);
// 	if (tmp[i] == '=')
// 		value = ft_substr(tmp, i + 1, ft_strlen(tmp) - i);
// 	else
// 		value = NULL;
// 	if (!var_name || !is_valid(var_name) || (tmp[i] == '=' && !tmp[i + 1]))
// 	{
// 		ft_putstr_fd("export: `", STDERR_FILENO);
// 		ft_putstr_fd(tmp, STDERR_FILENO);
// 		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
// 		free(tmp);
// 		free(var_name);
// 		free(value);
// 		command->exitcode = FAIL;
// 		return (command->exitcode);
// 	}
// 	command->args[1] = var_name;
// 	unset(command);
// 	if (value)
// 	{
// 		append_to_env(ft_strjoin(ft_strjoin(var_name, "="), value), command->env);
// 	}
// 	else
// 	{
// 		append_to_env(ft_strdup(var_name), command->env);
// 	}
// 	free(tmp);
// 	free(value);
// 	command->exitcode = 0;
// 	return (SUCCESS);
// }

int	export(t_Command *command)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	size_t	i;

	if (command->args[1] == NULL) // No arguments provided
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

	// Loop through all provided arguments
	for (i = 1; command->args[i] != NULL; i++)
	{
		tmp = ft_strdup(command->args[i]);
		if (!tmp)
		{
			perror("minishell: strdup failed");
			command->exitcode = FAIL;
			return (command->exitcode);
		}

		size_t equal_pos = 0;
		while (tmp[equal_pos] && tmp[equal_pos] != '=')
			equal_pos++;

		var_name = ft_substr(tmp, 0, equal_pos);
		if (tmp[equal_pos] == '=')
			value = ft_substr(tmp, equal_pos + 1, ft_strlen(tmp) - equal_pos - 1);
		else
			value = NULL;
		if (!var_name || !is_valid(var_name) || (tmp[equal_pos] == '=' && !tmp[equal_pos + 1] && !value))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(tmp, STDERR_FILENO);
			ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
			free(tmp);
			free(var_name);
			free(value);
			command->exitcode = FAIL;
			return (command->exitcode);
		}

		// Unset the variable if it exists, before adding it
		free(tmp);
		tmp = command->args[1];
		command->args[1] = var_name;
		unset(command);
		command->args[1] = tmp;
		
		// Add to environment
		if (value)
		{
			append_to_env(ft_strjoin(ft_strjoin(var_name, "="), value), command->env);
		}
		else
		{
			// Handle the case of NEW= (empty value)
			append_to_env(ft_strdup(var_name), command->env);
		}

		free(var_name);
		free(value);
	}

	command->exitcode = 0; // Indicate success
	return (SUCCESS);
}

