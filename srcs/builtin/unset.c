/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:41 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/09 17:31:43 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void remove_env_var(char **args, char **new, char ***envp_ptr)
{
	size_t i;
	size_t j;
	size_t len;
	char **var_name;

	len = get_str_len(*envp_ptr);
	i = 0;
	j = 0;
	while (i < len)
	{
		var_name = ft_split((*envp_ptr)[i], '=');
		if (ft_strncmp(var_name[0], args[1], ft_strlen(var_name[0]) + 1) != 0)
		{
			new[j] = (*envp_ptr)[i];
			j++;
		}
		else
		{
			//free((*envp_ptr)[i]);
		}
		delete_str_array(&var_name);
		i++;
	}
	new[j] = NULL;  // 새로운 배열의 끝을 NULL로 설정합니다.
}

int	unset(char **args, char ***envp_ptr)
{
	char **new_envp;
	size_t len;

	if (args[1] != NULL && args[1][0] == '-')
	{
		ft_putstr_fd("Error: unset, invalid option", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (FAIL);
	}

	if (args[1] != NULL)
	{
		len = get_str_len(*envp_ptr);  // 현재 환경 변수 배열의 길이 계산
		new_envp = ft_calloc(len + 1, sizeof(*new_envp));
		if (new_envp == NULL)
		{
			perror("Error: malloc failed");
			return (FAIL);
		}
		remove_env_var(args, new_envp, envp_ptr);  // 환경 변수 제거 및 새로운 배열 생성
		//free(*envp_ptr);  // 기존 환경 변수 배열 메모리 해제
		*envp_ptr = new_envp;  // 포인터를 새 배열로 업데이트
	}
	return (SUCCESS);
}
