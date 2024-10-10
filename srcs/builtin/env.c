/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:09 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 00:28:40 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int env(t_Command *command)
{
    if (command->args[0] == NULL)
    {
        command->exitcode = FAIL;
        return (command->exitcode);
    }

    if (command->args[1] != NULL && command->args[1][0] == '-')
    {
        ft_putstr_fd("Error: env, invalid argument ", STDERR_FILENO);
        ft_putstr_fd(command->args[1], STDERR_FILENO);
        command->exitcode = FAIL;
        return (command->exitcode);
    }
    
    print_strs(command->env);  // 환경 변수 출력
    command->exitcode = SUCCESS;  // 정상 종료 코드
    return (command->exitcode);
}

// int env(char **args, char ***envp_ptr)
// {
//     if (args[0] == NULL)
//         return (FAIL);

//     if (args[1] != NULL && args[1][0] == '-')
//     {
//         ft_putstr_fd("Error: env, invalid argument ", STDERR_FILENO);
//         ft_putstr_fd(args[1], STDERR_FILENO);
//         return (FAIL);
//     }
//     print_strs(*envp_ptr);
//     return (SUCCESS);
// }

char **create_env(char **curr_envp)
{
    size_t env_count;
    char **new_envp;

    env_count = get_str_len(curr_envp);
    new_envp = ft_calloc(env_count + 1, sizeof(*new_envp));
    if (new_envp == NULL)
    {
        perror("Error: malloc failed");
        return (NULL);
    }
    copy_envp(new_envp, curr_envp, env_count);
    return (new_envp);
}

void delete_str_array(char ***str_arr)
{
    size_t i;

    if (str_arr == NULL || *str_arr == NULL)
        return;

    i = 0;
    while ((*str_arr)[i] != NULL)
    {
        //free((*str_arr)[i]);
        (*str_arr)[i] = NULL;
        i++;
    }
    //free(*str_arr);
    *str_arr = NULL;
}

int append_to_env(char *str, char ***envp_ptr)
{
    size_t	env_len;
    char	**new_envp;

    env_len = get_str_len(*envp_ptr);
    new_envp = ft_calloc(env_len + 2, sizeof(*new_envp));
    if (new_envp == NULL)
    {
        perror("Error: malloc failed");
        return (FAIL);
    }
    copy_envp(new_envp, *envp_ptr, env_len);
    new_envp[env_len] = ft_strdup(str); // 새 문자열 추가
    if (new_envp[env_len] == NULL)
    {
        perror("Error: strdup failed");
		delete_str_array(&new_envp); // 기존 환경 배열 해제
        return (FAIL);
    }
    new_envp[env_len + 1] = NULL;
	delete_str_array(envp_ptr); // 기존 환경 배열 해제
    *envp_ptr = new_envp; // 새 환경 배열을 설정
	return (SUCCESS);
}

char	*get_env_value(const char *key, char **envp)
{
	size_t	i;
	size_t	k_len;

    if (key == NULL || envp == NULL)
        return (NULL);
    k_len = ft_strlen(key);
    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], key, k_len) == 0 && envp[i][k_len] == '=')
            return (ft_strchr(envp[i], '=') + 1); // = 이후의 값 반환
        i++;
    }
    return (NULL);
}
