/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:56:42 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/14 04:37:35 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_variable(char *input, t_Dollar *dol, char **env)
{
	dol->var_start = dol->i;
	while (ft_isalnum(input[dol->i]) || input[dol->i] == '_')
		dol->i++;
	dol->var_len = dol->i - dol->var_start;
	dol->var = ft_strndup(input + dol->var_start, dol->var_len);
	if (!dol->var)
		return (log_errors("Failed strndup in process_variable", ""));
	dol->var_value = get_env_value(dol->var, env);
	free(dol->var);
	if (!dol->var_value)
		dol->var_value = ft_strdup("");
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
		dol->output[dol->out_i++] = '$';
		return (SUCCESS);
	}
	return (expand_value(dol));
}
