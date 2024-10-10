/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:53:17 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 06:02:28 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	expand_value(t_Dollar *dol)
{
	size_t	value_len;
	char	*new_output;

	if (!dol->var_value)
		dol->var_value = ft_strdup("");
	if (!dol->var_value)
		return(FAIL);
	value_len = ft_strlen(dol->var_value);
	new_output = malloc((dol->out_len + value_len + 1) * sizeof(char));
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

// static int	for_exit_code(t_Dollar *dol, int last_exitcode)
// {
// 	dol->var_value = ft_itoa(last_exitcode);
// 	dol->i++;
// 	return (SUCCESS);
// }

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
// expand.c

// static int handle_single_quote(char *input, t_Dollar *dol)
// {
//     dol->i++; // 작은 따옴표 넘기기
//     while (input[dol->i] && input[dol->i] != '\'')
// 	{
//         dol->output[dol->out_i++] = input[dol->i++];
//     }
//     if (input[dol->i] == '\'') 
// 	{
//         dol->i++; // 닫는 작은 따옴표 넘기기
//     }
//     return (SUCCESS);
// }

// static int handle_double_quote(char *input, t_Dollar *dol, char **env, int last_exitcode)
// {
//     dol->i++; // 큰 따옴표 넘기기
//     while (input[dol->i] && input[dol->i] != '\"')
// 	{
//         if (input[dol->i] == '$')
// 		{
//             if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL)
//                 return (FAIL);
//         } else 
// 		{
//             dol->output[dol->out_i++] = input[dol->i++];
//         }
//     }
//     if (input[dol->i] == '\"') 
//         dol->i++; // 닫는 큰 따옴표 넘기기
//     return (SUCCESS);
// }

static int	handle_single_quote(char *input, t_Dollar *dol)
{
	dol->i++; // 작은 따옴표 넘기기
	while (input[dol->i] && input[dol->i] != '\'')
	{
		// 작은 따옴표 안에서는 변수 확장을 무시하고 그대로 출력
		dol->output[dol->out_i++] = input[dol->i++];
		// 버퍼 크기 확인 및 재할당 필요 시 수행
		if (dol->out_i >= dol->out_len - 1)
		{
			dol->out_len *= 2;
			dol->output = realloc(dol->output, dol->out_len);
			if (!dol->output)
			{
				log_errors("Realloc failed", "");
				return (FAIL);
			}
		}
	}
	if (input[dol->i] == '\'')
		dol->i++; // 닫는 작은 따옴표 넘기기
	return (SUCCESS);
}

// handle_double_quote 함수 수정: 큰 따옴표 안에서는 변수 확장만 수행
static int	handle_double_quote(char *input, t_Dollar *dol, char **env, int last_exitcode)
{
	dol->i++; // 큰 따옴표 넘기기
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
			// 버퍼 크기 확인 및 재할당 필요 시 수행
			if (dol->out_i >= dol->out_len - 1)
			{
				dol->out_len *= 2;
				dol->output = realloc(dol->output, dol->out_len);
				if (!dol->output)
				{
					log_errors("Realloc failed", "");
					return (FAIL);
				}
			}
		}
	}
	if (input[dol->i] == '\"')
		dol->i++; // 닫는 큰 따옴표 넘기기
	return (SUCCESS);
}


int for_dollar_sign(char *input, t_Dollar *dol, char **env, int last_exitcode) 
{
    dol->i++;
    if (input[dol->i] == '{') 
	{
        if (for_curly_braces(input, dol, env, last_exitcode) == FAIL)
            return (FAIL);
    } else if (input[dol->i] == '?') 
	{
        if (for_exit_code(dol, last_exitcode) == FAIL)
            return (FAIL);
    } else if (ft_isalpha(input[dol->i]) || input[dol->i] == '_') 
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
        // 달러 기호 뒤에 유효한 변수가 없으면 달러 기호 그대로 출력
        dol->output[dol->out_i++] = '$';
        // dol->i는 이미 증가되었으므로 그대로 진행
        return (SUCCESS);
    }
    return (expand_value(dol));
}


char *find_dollar_signs(char *input, char **env, int last_exitcode) 
{
    t_Dollar dol;

    if (!input)
        return (NULL);

    dol.len = ft_strlen(input);
    dol.out_len = dol.len + 1;
    dol.output = malloc(dol.out_len);
    if (!dol.output) 
	{
        log_errors("Malloc failed", "");
        return (NULL);
    }
    dol.out_i = 0;
    dol.i = 0;

    while (dol.i < dol.len) 
	{
        if (input[dol.i] == '\'') 
		{
            if (handle_single_quote(input, &dol) == FAIL)
                return (NULL);
        } 
		else if (input[dol.i] == '\"') 
		{
            if (handle_double_quote(input, &dol, env, last_exitcode) == FAIL)
                return (NULL);
        } 
		else if (input[dol.i] == '$') 
		{
            if (for_dollar_sign(input, &dol, env, last_exitcode) == FAIL) 
			{
                expand_error(input);
				free(dol.output);
                return (NULL);
            }
        } 
		else 
		{
            dol.output[dol.out_i++] = input[dol.i++];
            // 출력 버퍼의 크기를 확인하고 필요하면 재할당
            if (dol.out_i >= dol.out_len - 1)
			{
                dol.out_len *= 2;
                dol.output = realloc(dol.output, dol.out_len);
                if (!dol.output) 
				{
                    log_errors("Realloc failed", "");
                    return (NULL);
                }
            }
        }
    }
    dol.output[dol.out_i] = '\0';
    return (dol.output);
}
