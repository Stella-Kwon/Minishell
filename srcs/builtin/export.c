/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:21 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/12 11:27:56 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// 변수 이름이 유효한지 확인하는 함수
static bool	is_valid(char *var_name)
{
	size_t	i;
	
	i = 1;
	if (!var_name || var_name[0] == '='|| !(ft_isalpha(var_name[0]))) // 변수명이 없는 경우나 '='로 시작하는 경우
		return (false);
	while (var_name[i] != '\0' && var_name[i] != '=')
	{
		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_')) // 알파벳, 숫자, '_'만 허용
			return (false);
		i++;
	}
	return (true);
}

// // 환경 변수를 출력하는 함수
// static int	print_env_var(char *env_var)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!env_var)
// 		return (FAIL);
// 	ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 	while (env_var[i] != '\0')
// 	{
// 		ft_putchar_fd(env_var[i], STDOUT_FILENO);
// 		if (env_var[i] == '=')
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		i++;
// 	}
// 	if (ft_strchr(env_var, '=') != NULL)
// 		ft_putstr_fd("\"", STDOUT_FILENO);
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	return (SUCCESS);
// }

// // 환경 변수를 정렬하여 출력하는 함수
// static int	print_sorted_envp(char **envp)
// {
// 	size_t	i;
// 	size_t	len;
// 	char	**sorted_envp;

// 	if (!envp)
// 		return (FAIL);
// 	len = get_str_len(envp);
// 	sorted_envp = ft_calloc(len + 1, sizeof(*sorted_envp));
// 	if (!sorted_envp)
// 	{
// 		perror("Error: malloc failed");
// 		return (FAIL);
// 	}
// 	if (copy_envp(envp, sorted_envp, len) == FAIL)
// 		return (FAIL);
// 	sort_envp(len, sorted_envp);
// 	i = 0;
// 	while (i < len)
// 	{
// 		if (sorted_envp[i][0] != '_' && sorted_envp[i][1] != '=')
// 		{
// 			if (print_env_var(sorted_envp[i]) == FAIL)
// 			{
// 				delete_str_array(&sorted_envp);
// 				return (FAIL);
// 			}
// 		}
// 		i++;
// 	}
// 	delete_str_array(&sorted_envp);
// 	return (SUCCESS);
// }

// export 명령을 처리하는 함수
// void	export_check(t_Command *command)
// {
// 	if (command->args[1] == NULL)
// 	{
// 		print_sorted_envp(command->env);
// 		return ;
// 	}
// 	// 변수명이 유효하지 않거나 '='로 시작하면 에러 처리
// 	if (!is_valid(command->args[1]) || command->args[1][0] == '=' || \
// 				ft_strchr(command->args[1], '-') != NULL)
// 	{
// 		ft_putstr_fd("export: `", STDERR_FILENO);
// 		ft_putstr_fd(command->args[1], STDERR_FILENO);
// 		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
// 		command->exitcode = 1;
// 		return ;
// 	}
// 	if (ft_strchr(command->args[1], '=') != NULL)
// 	{
// 		if (export(command) == FAIL)
// 			return ;
// 	}
// }

// 환경 변수를 추가하는 함수
int	export(t_Command *command)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = ft_strdup(command->args[1]);
	if (!tmp)
	{
		perror("Error: strdup failed");
		command->exitcode = FAIL;
		return (command->exitcode);
	}
	while (tmp[i] && tmp[i] != '=')
		i++;
	var_name = ft_substr(tmp, 0, i);  // 변수 이름 추출
	if (tmp[i] == '=')
		value = ft_substr(tmp, i + 1, ft_strlen(tmp) - i);  // 값 추출
	else
		value = NULL;  // 값이 없는 경우(NULL)

	if (!var_name || !is_valid(var_name) || (tmp[i] == '=' && !tmp[i + 1])) 
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(tmp, STDERR_FILENO);
		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
		free(tmp);
		free(var_name);
		free(value);
		command->exitcode = FAIL;
		return (command->exitcode);
	}

	// 기존 변수 제거 후 새로 추가
	command->args[1] = var_name;
	unset(command);  // 기존 변수가 있으면 제거
	if (value)
	{
		append_to_env(ft_strjoin(ft_strjoin(var_name, "="), value), command->env);
	}
	else
		append_to_env(ft_strdup(var_name), command->env);

	free(tmp);
	free(value);
	command->exitcode = SUCCESS;
	return (command->exitcode);
}



// #include "../../includes/minishell.h"

// // 변수 이름이 유효한지 확인하는 함수
// static bool	is_valid(char *var_name)
// {
// 	size_t	i;

// 	if (!var_name || !(ft_isalpha(var_name[0]) || var_name[0] == '_'))
// 		return (false);  // 첫 글자는 알파벳 또는 _이어야 함
// 	i = 1;
// 	while (var_name[i] != '\0' && var_name[i] != '=')
// 	{
// 		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_'))
// 			return (false);  // 숫자나 _가 아닌 다른 문자는 유효하지 않음
// 		i++;
// 	}
// 	return (true);
// }

// // 환경 변수를 출력하는 함수
// static int	print_env_var(char *env_var)
// {
// 	size_t	i;

// 	if (!env_var)
// 		return (FAIL);
// 	i = 0;
// 	ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 	while (env_var[i] != '\0')
// 	{
// 		ft_putchar_fd(env_var[i], STDOUT_FILENO);
// 		if (env_var[i] == '=')
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		i++;
// 	}
// 	if (ft_strchr(env_var, '=') != NULL)
// 		ft_putstr_fd("\"", STDOUT_FILENO);
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	return (SUCCESS);
// }

// // 환경 변수를 정렬하여 출력하는 함수
// static int	print_sorted_envp(char **envp)
// {
// 	size_t	i;
// 	size_t	len;
// 	char	**sorted_envp;

// 	if (!envp)
// 		return (FAIL);
// 	len = get_str_len(envp);
// 	sorted_envp = ft_calloc(len + 1, sizeof(*sorted_envp));
// 	if (!sorted_envp)
// 	{
// 		perror("Error: malloc failed");
// 		return (FAIL);
// 	}
// 	if (copy_envp(envp, sorted_envp, len) == FAIL)
// 		return (FAIL);
// 	sort_envp(len, sorted_envp);
// 	i = 0;
// 	while (i < len)
// 	{
// 		if (sorted_envp[i][0] != '_' && sorted_envp[i][1] != '=')
// 		{
// 			if (print_env_var(sorted_envp[i]) == FAIL)
// 			{
// 				delete_str_array(&sorted_envp);
// 				return (FAIL);
// 			}
// 		}
// 		i++;
// 	}
// 	delete_str_array(&sorted_envp);
// 	return (SUCCESS);
// }

// // export 명령을 처리하는 함수
// void	export_check(t_Command *command)
// {
// 	if (command->args[1] == NULL)
// 	{
// 		print_sorted_envp(command->env);
// 		return ;
// 	}
// 	if (!is_valid(command->args[1]) || command->args[1][0] == '=')
// 	{
// 		ft_putstr_fd("export: `", STDERR_FILENO);
// 		ft_putstr_fd(command->args[1], STDERR_FILENO);
// 		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
// 		command->exitcode = 1;
// 		return ;
// 	}
// 	if (ft_strchr(command->args[1], '=') != NULL)
// 	{
// 		if (export(command) == FAIL)
// 			return ;
// 	}
// }

// // 환경 변수를 추가하는 함수
// int	export(t_Command *command)
// {
// 	char	*var_name;
// 	char	*value;
// 	char	*tmp;
// 	size_t	i;

// 	i = 0;
// 	tmp = ft_strdup(command->args[1]);
// 	if (!tmp)
// 	{
// 		perror("Error: strdup failed");
// 		command->exitcode = FAIL;
// 		return (command->exitcode);
// 	}
// 	while (tmp[i] && tmp[i] != '=')
// 		i++;
// 	var_name = ft_substr(tmp, 0, i);  // 변수 이름 추출
// 	if (tmp[i] == '=')
// 		value = ft_substr(tmp, i + 1, ft_strlen(tmp) - i);  // 값 추출
// 	else
// 		value = NULL;  // 값이 없는 경우(NULL)

// 	if (!var_name || (tmp[i] == '=' && !tmp[i + 1]))  // '='만 있는 경우
// 	{
// 		ft_putstr_fd("export: `", STDERR_FILENO);
// 		ft_putstr_fd(tmp, STDERR_FILENO);
// 		ft_putstr_fd("`: not a valid identifier\n", STDERR_FILENO);
// 		free(tmp);
// 		free(var_name);
// 		free(value);
// 		command->exitcode = FAIL;
// 		return (command->exitcode);
// 	}

// 	// 기존 변수 제거 후 새로 추가
// 	command->args[1] = var_name;
// 	unset(command);  // 기존 변수가 있으면 제거
// 	if (value)
// 	{
// 		append_to_env(ft_strjoin(var_name, "="), &command->env);
// 		append_to_env(ft_strjoin(var_name, value), &command->env);
// 	}
// 	else
// 		append_to_env(ft_strdup(var_name), &command->env);

// 	free(tmp);
// 	free(value);
// 	command->exitcode = SUCCESS;
// 	return (command->exitcode);
// }
