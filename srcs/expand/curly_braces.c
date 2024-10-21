/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   curly_braces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 01:52:12 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/14 05:45:59 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_curly_braces_syntax(char *input, t_Dollar *dol)
{
	while (input[dol->i] != '}' && input[dol->i] != '\0')
	{
		if (!ft_isalnum(input[dol->i]) && input[dol->i] != '_' && \
			input[dol->i] != '?')
			return (FAIL);
		dol->i++;
	}
	if (input[dol->i] == '}')
	{
		return (SUCCESS);
	}
	else
	{
		return (FAIL);
	}
}

static int	exitcode_or_var(char *input, t_Dollar *dol, char **env, \
							int last_exitcode)
{
	(void)env;
	if (input[dol->i] == '?')
	{
		if (for_exit_code(dol, last_exitcode) == FAIL)
			return (FAIL);
		if (input[dol->i] == '}')
		{
			dol->i++;
			return (SUCCESS);
		}
		return (FAIL);
	}
	else
	{
		dol->var_start = dol->i;
		if (validate_curly_braces_syntax(input, dol) == FAIL)
			return (FAIL);
		return (SUCCESS);
	}
}

int	for_curly_braces(char *input, t_Dollar *dol, char **env, \
					int last_exitcode)
{
	char	*var;

	dol->i++;
	if (exitcode_or_var(input, dol, env, last_exitcode) == FAIL)
		return (FAIL);
	if (input[dol->i] == '}')
	{
		dol->var_len = dol->i - dol->var_start;
		var = ft_strndup(input + dol->var_start, dol->var_len);
		if (!var)
			return (log_errors("Failed strdup in for curly braces", ""));
		dol->var_value = get_env_value(var, env);
		free(var);
		dol->i++;
	}
	return (SUCCESS);
}
