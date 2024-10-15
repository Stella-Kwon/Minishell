/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:17 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 04:46:04 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	expand_value(t_Dollar *dol)
{
	size_t	value_len;
	char	*new_output;

	value_len = ft_strlen(dol->var_value);
	new_output = malloc((dol->out_len + value_len + 1) * sizeof(char));
	if (!dol->var_value)
		dol->var_value = ft_strdup("");
	if (!dol->var_value)
	{
		log_errors("Failed strdup in expand_value", "");
		return (FAIL);
	}
	if (!new_output)
	{
		log_errors("Failed malloc in expand_value", "");
		free(dol->output);
		return (FAIL);
	}
	ft_strcpy(new_output, dol->output);
	ft_strcpy(new_output + dol->out_i, dol->var_value);
	free(dol->output);
	dol->output = new_output;
	dol->out_i += value_len;
	dol->out_len = dol->out_i;
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
	if (handle_special_character(input, dol, env, last_exitcode) == FAIL)
		return (FAIL);
	dol->output[dol->out_i++] = input[dol->i++];
	if (dol->out_i >= dol->out_len - 1)
	{
		dol->output = ft_realloc_single(dol->output, dol->out_i, \
										(int *)&dol->out_len);
		if (!dol->output)
		{
			log_errors("Failed realloc in process_character", "");
			return (FAIL);
		}
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

char	*find_dollar_signs(char *input, char **env, int last_exitcode)
{
	t_Dollar	dol;

	dol.out_i = 0;
	dol.i = 0;
	dol.len = ft_strlen(input);
	dol.out_len = dol.len + 1;
	dol.output = malloc(dol.out_len);
	if (!input || !dol.output)
	{
		log_errors("Failed malloc in expand", "");
		return (NULL);
	}
	if (process_quotes(input, &dol, env, last_exitcode) == FAIL) 
	{
		free(dol.output);
		return (NULL);
	}
	dol.output[dol.out_i] = '\0';
	return (dol.output);
}
