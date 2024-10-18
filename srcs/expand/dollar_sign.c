/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:56:42 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 22:46:57 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_variable(char *input, t_Dollar *dol, char **env)
{
	char	*var;
	char	*env_val;

	dol->var_start = dol->i;
	while (ft_isalnum(input[dol->i]) || input[dol->i] == '_')
		dol->i++;
	dol->var_len = dol->i - dol->var_start;
	var = ft_strndup(input + dol->var_start, dol->var_len);
	if (!var)
		return (log_errors("Failed strndup in process_variable", ""));
	env_val = get_env_value(var, env);
	free(var);
	if (!env_val)
	{
		dol->var_value = NULL;
		return (SUCCESS);
	}
	dol->var_value = ft_strdup(env_val);
	if (!dol->var_value)
		return (log_errors("Failed strndup in process_variable", ""));
	return (SUCCESS);
}

int	for_dollar_sign(char *input, t_Dollar *dol, char **env, \
					int last_exitcode)
{
	dol->i++;
	if (input[dol->i] == '{')
	{
		if (for_curly_braces(input, dol, env, last_exitcode) == FAIL)
			return (FAIL);
	}
	else if (input[dol->i] == '?')
	{
		if (for_exit_code(dol, last_exitcode) == FAIL)
			return (FAIL);
	}
	else if (ft_isalpha(input[dol->i]) || input[dol->i] == '_')
	{
		if (process_variable(input, dol, env) == FAIL)
			return (FAIL);
	}
	else
	{
		dol->tmp[dol->tmp_i++] = '$';
		return (SUCCESS);
	}
	return (expand_value(dol));
}
