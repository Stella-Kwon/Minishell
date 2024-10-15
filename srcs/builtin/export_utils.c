/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 06:02:36 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 23:57:11 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static int	initialize_sorted_envp(char ***sorted_envp, char **envp, \
									size_t len)
{
	*sorted_envp = ft_calloc(len + 1, sizeof(**sorted_envp));
	if (!(*sorted_envp))
	{
		log_errors("Failed calloc in init sorted envp", "");
		return (FAIL);
	}
	if (copy_envp(*sorted_envp, envp, len) == FAIL)
	{
		delete_str_array(sorted_envp);
		return (FAIL);
	}
	sort_envp(len, *sorted_envp);
	return (SUCCESS);
}

static int	output_sorted_envp(char **sorted_envp, size_t len)
{
	size_t	i;

	i = 0;

	while (i < len)
	{
		if (sorted_envp[i][0] != '_' && sorted_envp[i][1] != '=')
		{
			if (print_env_var(sorted_envp[i]) == FAIL)
				return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

int	print_sorted_envp(char **envp)
{
	size_t	len;
	int		result;
	char	**sorted_envp;

	sorted_envp = NULL;
	len = get_str_len(envp);
	result = output_sorted_envp(sorted_envp, len);
	if (!envp)
		return (FAIL);
	if (initialize_sorted_envp(&sorted_envp, envp, len) == FAIL)
		return (FAIL);
	delete_str_array(&sorted_envp);
	return (result);
}

int	add_env_item(t_Command *command, char *var_name, char *value)
{
	char	*tmp;
	char	*complete;

	tmp = command->args[1];
	command->args[1] = var_name;
	if (unset(command) == FAIL)
		return (FAIL);
	command->args[1] = tmp;
	if (value)
		complete = ft_strjoin3(var_name, "=", value);
	else
		complete = ft_strdup(var_name);
	if (!complete)
	{
		log_errors("Strjoin3 or strdup failed", "");
		return (FAIL);
	}
	append_to_env(complete, command->env);
	free(complete);
	return (SUCCESS);
}
