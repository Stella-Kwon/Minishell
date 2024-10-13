/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:17 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 19:38:22 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	expand_value(t_Dollar *dol)
{
	size_t	value_len;
	char	*new_output;

	value_len = ft_strlen(dol->var_value);
	new_output = malloc((dol->out_len + value_len + 1) * sizeof(char));
	if (!dol->var_value)
		dol->var_value = ft_strdup("");
	if (!dol->var_value)
		return (FAIL);
	if (new_output == NULL)
	{
		log_errors("Malloc failed", "");
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

static int	for_exit_code(t_Dollar *dol, int last_exitcode)
{
	char	*exit_code_str;

	exit_code_str = ft_itoa(last_exitcode);
	if (!exit_code_str)
		return (FAIL);
	dol->var_value = exit_code_str;
	dol->i++;
	return (SUCCESS);
}

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

static int	for_curly_braces(char *input, t_Dollar *dol, char **env, \
							int last_exitcode)
{
	dol->i++;
	if (input[dol->i] == '?')
	{
		if (for_exit_code(dol, last_exitcode) == FAIL)
			return (FAIL);
		if (input[dol->i] == '}')
		{
			dol->i++;
			return (SUCCESS);
		}
		else
			return (FAIL);
	}
	else
	{
		dol->var_start = dol->i;
		if (validate_curly_braces_syntax(input, dol) == FAIL)
			return (FAIL);
	}
	if (input[dol->i] == '}')
	{
		dol->var_len = dol->i - dol->var_start;
		dol->var = ft_strndup(input + dol->var_start, dol->var_len);
		if (!dol->var)
			return (FAIL);
		dol->var_value = get_env_value(dol->var, env);
		free(dol->var);
		dol->i++;
	}
	return (SUCCESS);
}

static int	handle_single_quote(char *input, t_Dollar *dol)
{
	dol->i++;
	while (input[dol->i] && input[dol->i] != '\'')
	{
		dol->output[dol->out_i++] = input[dol->i++];
		if (dol->out_i >= dol->out_len - 1)
		{
			dol->output = ft_realloc_single(dol->output, dol->out_i, \
											(int *)&dol->out_len);
			if (!dol->output)
			{
				log_errors("Realloc failed", "");
				return (FAIL);
			}
		}
	}
	if (input[dol->i] == '\'')
		dol->i++;
	return (SUCCESS);
}

static int	handle_double_quote(char *input, t_Dollar *dol, char **env, \
								int last_exitcode)
{
	dol->i++;
	while (input[dol->i] && input[dol->i] != '\"')
	{
		if (input[dol->i] == '$')
		{
			if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL)
				return (FAIL);
		}
		else
		{
			dol->output[dol->out_i++] = input[dol->i++];
			if (dol->out_i >= dol->out_len - 1)
			{
				dol->output = ft_realloc_single(dol->output, dol->i, \
												(int *)&dol->out_len);
				if (!dol->output)
				{
					log_errors("Realloc failed", "");
					return (FAIL);
				}
			}
		}
	}
	if (input[dol->i] == '\"')
		dol->i++;
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
		dol->var_start = dol->i;
		while (ft_isalnum(input[dol->i]) || input[dol->i] == '_')
			dol->i++;
		dol->var_len = dol->i - dol->var_start;
		dol->var = ft_strndup(input + dol->var_start, dol->var_len);
		dol->var_value = get_env_value(dol->var, env);
		if (!dol->var)
			return (FAIL);
		free(dol->var);
		if (!dol->var_value)
			dol->var_value = ft_strdup("");
	}
	else
	{
		dol->output[dol->out_i++] = '$';
		return (SUCCESS);
	}
	return (expand_value(dol));
}

static int	process_character(char *input, t_Dollar *dol, char **env, \
								int last_exitcode)
{
	if (input[dol->i] == '\'') 
	{
		if (handle_single_quote(input, dol) == FAIL)
			return (FAIL);
	} else if (input[dol->i] == '\"') 
	{
		if (handle_double_quote(input, dol, env, last_exitcode) == FAIL)
			return (FAIL);
	} else if (input[dol->i] == '$')
	{
		if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL) 
		{
			expand_error(input);
			return (FAIL);
		}
	}
	else
	{
		dol->output[dol->out_i++] = input[dol->i++];
		if (dol->out_i >= dol->out_len - 1)
		{
			dol->output = ft_realloc_single(dol->output, dol->out_i, \
											(int *)&dol->out_len);
			if (!dol->output)
			{
				log_errors("Realloc failed", "");
				return (FAIL);
			}
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
	t_Dollar dol;

	dol.out_i = 0;
	dol.i = 0;
	dol.len = ft_strlen(input);
	dol.out_len = dol.len + 1;
	dol.output = malloc(dol.out_len);
	if (!input || !dol.output)
	{
		log_errors("Malloc failed", "");
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
