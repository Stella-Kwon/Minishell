#include "../../includes/minishell.h"

static int	expand_value(t_Dollar *dol)
{
	size_t	value_len;
	char	*new_output;

	if (!dol->var_value)
		return(SUCCESS);
	value_len = strlen(dol->var_value);
	new_output = malloc((dol->out_len + value_len) * sizeof(char));
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
	dol->out_len += value_len;
	return (SUCCESS);
}

static int	for_exit_code(t_Dollar *dol)
{
	size_t	exit_status;

	exit_status = 0;
	dol->var_value = ft_itoa(WEXITSTATUS(exit_status));
	return (SUCCESS);
}

static int	for_curly_braces(char *input, t_Dollar *dol, char **env)
{
	dol->i++;
	if (input[dol->i] == '?')
	{
		if (for_exit_code(dol) == FAIL)
			return (FAIL);
		dol->i++;
	}
	else
	{
		dol->var_start = dol->i;
		while (ft_isalnum(input[dol->i]) || input[dol->i] == '_')
			dol->i++;
	}
	if (input[dol->i] == '}')
	{
		dol->var_len = dol->i - dol->var_start;
		dol->var = ft_strndup(input + dol->var_start, dol->var_len);
		dol->var_value = get_env_value(dol->var, env);
		free(dol->var);
		dol->i++;
	}
	else
		return (FAIL);
	return (SUCCESS);
}

static int	for_dollar_sign(char *input, t_Dollar *dol, char **env)
{
	dol->i++;
	if (input[dol->i] == '{')
	{
		if (for_curly_braces(input, dol, env) == FAIL)
			return (FAIL);
	}
	else if (input[dol->i] == '?')
	{
		if (for_exit_code(dol) == FAIL)
			return (FAIL);
	}
	else
	{
		dol->var_start = dol->i;
		while (ft_isalnum(input[dol->i]) || input[dol->i] == '_')
			dol->i++;
		dol->var_len = dol->i - dol->var_start;
		dol->var = ft_strndup(input + dol->var_start, dol->var_len);
		dol->var_value = get_env_value(dol->var, env);
		free(dol->var);
	}
	if (dol->var_value == NULL)
		return (SUCCESS);
	return (expand_value(dol));
}

char	*find_dollar_signs(char *input, char **env)
{
	t_Dollar dol;

	if (!input)
		return (input);
	dol.len = ft_strlen(input);
	dol.out_len = dol.len + 1;
	dol.output = malloc(dol.out_len * sizeof(char));
	if (dol.output == NULL)
	{
		log_errors("Malloc failed", "");
		return (input);
	}
	dol.out_i = 0;
	dol.i = 0;
	while (dol.i < dol.len)
	{
		if (input[dol.i] == '$')
		{
			if (for_dollar_sign(input, &dol, env) == FAIL)
				return (input);
		}
		else
		{
			dol.output[dol.out_i++] = input[dol.i];
			dol.i++;
		}
	}
	dol.output[dol.out_i] = '\0';
	return (dol.output);
}

