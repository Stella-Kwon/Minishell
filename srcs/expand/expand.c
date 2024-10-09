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

static int	for_exit_code(t_Dollar *dol, int last_exit_code)
{
	dol->var_value = ft_itoa(last_exit_code);
	dol->i++;
	return (SUCCESS);
}

static int	validate_curly_braces_syntax(char *input, t_Dollar *dol)
{
	// ${} 내부에 올바르지 않은 문자가 있는지 검사
	while (input[dol->i] != '}' && input[dol->i] != '\0')
	{
		// 유효한 문자(알파벳, 숫자, '_', '?')만 허용
		if (!ft_isalnum(input[dol->i]) && input[dol->i] != '_' && input[dol->i] != '?')
			return (FAIL); // 오류 발생
		dol->i++;
	}
	return (input[dol->i] == '}') ? SUCCESS : FAIL; // 중괄호 닫힘 확인
}

static int	for_curly_braces(char *input, t_Dollar *dol, char **env, \
							int last_exit_code)
{
	dol->i++;
	if (input[dol->i] == '?')
	{
		if (for_exit_code(dol, last_exit_code) == FAIL)
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
		dol->var_value = get_env_value(dol->var, env);
		free(dol->var);
		dol->i++;
	}
	// else
	// 	return (FAIL);
	return (SUCCESS);
}

static int	for_dollar_sign(char *input, t_Dollar *dol, char **env, int last_exit_code)
{
	dol->i++;
	if (input[dol->i] == '{')
	{
		if (for_curly_braces(input, dol, env, last_exit_code) == FAIL)
			return (FAIL);
	}
	else if (input[dol->i] == '?')
	{
		if (for_exit_code(dol, last_exit_code) == FAIL)
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

char	*find_dollar_signs(char *input, char **env, int last_exit_code)
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
			if (for_dollar_sign(input, &dol, env, last_exit_code) == FAIL)
			{
				expand_error(input);
				return (NULL);
			}
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
