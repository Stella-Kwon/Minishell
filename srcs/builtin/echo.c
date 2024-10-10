/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:30:54 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 06:03:51 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_n(char *str)
{
	size_t	i;

	if (str[0] == '-' && str[1] == 'n')
	{
		i = 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}

static void	print_args(char **args, size_t i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

// static void	print_args(char **args, size_t i, char **envp, int last_exitcode)
// {
// 	char	*expanded;
// 	char	*no_quotes;

// 	while (args[i])
// 	{
// 		expanded = find_dollar_signs(args[i], envp, last_exitcode);
// 		if (!expanded)
// 		{
// 			// 확장 실패 시, 오류 처리 및 계속
// 			ft_putstr_fd("MINISHELL: Variable expansion failed\n", STDERR_FILENO);
// 			i++;
// 			continue;
// 		}
// 		no_quotes = remove_quotes(expanded);
// 		if (!no_quotes)
// 		{
// 			// 메모리 할당 실패 시, 오류 처리 및 계속
// 			ft_putstr_fd("MINISHELL: Memory allocation failed\n", STDERR_FILENO);
// 			free(expanded);
// 			i++;
// 			continue;
// 		}
// 		ft_putstr_fd(no_quotes, STDOUT_FILENO);
// 		if (args[i + 1])
// 			ft_putchar_fd(' ', STDOUT_FILENO);
// 		free(expanded);
// 		free(no_quotes);
// 		i++;
// 	}
// }

// // 수정된 echo 함수
// int	echo(t_Command *command)
// {
// 	int		n_option;
// 	char	**envp;
// 	int		last_exitcode;

// 	n_option = false;
// 	envp = command->env;
// 	last_exitcode = command->exitcode; // 이전 명령어의 종료 코드

// 	if (command->args == NULL || command->args[1] == NULL)
//     {
//         ft_putchar_fd('\n', STDOUT_FILENO);
//         command->exitcode = SUCCESS;
//         return (command->exitcode);
//     }
// 	if (command->args && command->args[1] != NULL)
// 	{
// 		if (is_n(command->args[1]))
// 			n_option = true;
// 		print_args(command->args, 1 + n_option, envp, last_exitcode);
// 	}
// 	if (!n_option)
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 	command->exitcode = SUCCESS;  // 정상 종료 시 0
// 	return (command->exitcode);
// }

// int echo(t_Command *command)
// {
//     int n_option;
	
// 	n_option = false;

//     if (command->args[1] != NULL)
//     {
//         if (is_n(command->args[1]) == true)
//             n_option = true;
//         print_args(command->args, 1 + n_option);
//     }
//     if (!n_option)
//         ft_putchar_fd('\n', STDOUT_FILENO);

//     command->exitcode = SUCCESS;  // 정상 종료 시 0
//     return (command->exitcode);
// }


int echo(t_Command *command)
{
    int n_option;
	n_option = false;

    if (command->args == NULL || command->args[1] == NULL)
    {
        ft_putchar_fd('\n', STDOUT_FILENO);
        command->exitcode = SUCCESS;
        return (command->exitcode);
    }
    if (is_n(command->args[1]) == true)
        n_option = true;
    print_args(command->args, 1 + n_option);
    if (!n_option)
        ft_putchar_fd('\n', STDOUT_FILENO);
    command->exitcode = SUCCESS;  // 정상 종료 시 0
    return (command->exitcode);
}



// char* process_quotes(char *input) {
//     size_t len = strlen(input);
//     char *result = malloc(len + 1);
//     if (!result) return NULL;

//     size_t j = 0;
//     for (size_t i = 0; i < len; i++) {
//         if (input[i] != '\'' && input[i] != '\"') {
//             result[j++] = input[i];  // 따옴표가 아니면 복사
//         }
//     }
//     result[j] = '\0';
//     return result;
// }

