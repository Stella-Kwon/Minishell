/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:17 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 22:51:35 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	expand_value(t_Dollar *dol)
{
	size_t	value_len;
	char	*new_output;

	if (!dol->var_value || !dol->tmp)
		return (SUCCESS);
	value_len = ft_strlen(dol->var_value);
	new_output = malloc((dol->tmp_len + value_len + 1) * sizeof(char));
	if (!new_output)
	{
		log_errors("Failed malloc in expand_value", "");
		free(dol->var_value);
		free(dol->tmp);
		return (FAIL);
	}
	dol->tmp[dol->tmp_i] = '\0';
	ft_strcpy(new_output, dol->tmp);
	ft_strcpy(new_output + dol->tmp_i, dol->var_value);
	free(dol->var_value);
	dol->var_value = NULL;
	free(dol->tmp);
	dol->tmp = new_output;
	dol->tmp_i += value_len;
	dol->tmp_len += value_len;
	return (SUCCESS);
}

static int	handle_special_character(char *input, t_Dollar *dol, char **env, \
									int last_exitcode)
{
	if (input[dol->i] == '\'')
	{
		if (handle_single_quote(input, dol) == FAIL)
			return (FAIL);
	}
	else if (input[dol->i] == '\"')
	{
		if (handle_double_quote(input, dol, env, last_exitcode) == FAIL)
			return (FAIL);
	}
	else if (input[dol->i] == '$')
	{
		if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL)
		{
			expand_error(input);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

static int	process_character(char *input, t_Dollar *dol, char **env, \
							int last_exitcode)
{
	char	*tmp;

	if (handle_special_character(input, dol, env, last_exitcode) == FAIL)
		return (FAIL);
	dol->tmp[dol->tmp_i++] = input[dol->i++];
	if (dol->tmp_i >= dol->tmp_len - 1)
	{
		tmp = ft_realloc_single(dol->tmp, dol->tmp_i, (int *)&dol->tmp_len);
		if (!tmp)
		{
			log_errors("Failed realloc in process_character", "");
			return (FAIL);
		}
		dol->tmp = tmp;
	}
	return (SUCCESS);
}

static int	process_quotes(char *input, t_Dollar *dol, char **env, \
							int last_exitcode)
{
	while (dol->i < dol->len)
	{
		if (input[dol->i] == '\'')
		{
			if (handle_single_quote(input, dol) == FAIL)
				return (FAIL);
			dol->i++;
		}
		else if (input[dol->i] == '\"')
		{
			if (handle_double_quote(input, dol, env, last_exitcode) == FAIL)
				return (FAIL);
		}
		else
		{
			if (process_character(input, dol, env, last_exitcode) == FAIL)
				return (FAIL);
		}
	}
	return (SUCCESS);
}

int	find_dollar_signs(char **in_out, char **env, int last_exitcode)
{
	t_Dollar	dol;

	if (!in_out || !(*in_out))
		return (FAIL);
	dol.tmp_i = 0;
	dol.i = 0;
	dol.len = ft_strlen(*in_out);
	dol.tmp_len = dol.len + 1;
	dol.var_value = NULL;
	dol.tmp = malloc((dol.tmp_len) * sizeof(char));
	if (!dol.tmp)
		return (log_errors("Failed malloc in expand", ""));
	if (process_quotes(*in_out, &dol, env, last_exitcode) == FAIL)
	{
		free(dol.tmp);
		if (dol.var_value)
			free(dol.var_value);
		return (FAIL);
	}
	dol.tmp[dol.tmp_i] = '\0';
	free(*in_out);
	*in_out = dol.tmp;
	if (dol.var_value)
		free(dol.var_value);
	return (SUCCESS);
}
