/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:15 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/10 15:55:02 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void print_exit_err_msg(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
    exit (255);
}

// int	mini_exit(char **args, char ***envp_ptr)
// {
// 	int	exit_status;
// 	size_t	i;

// 	i = 0;
// 	(void)envp_ptr;
// 	if (args[1] == NULL)
// 		exit(0);
// 	while (args[1][i] != '\0')
// 	{
// 		if (!ft_isdigit(args[1][i]))
// 			print_exit_err_msg(args[1]);
// 		i++;
// 	}
// 	if (args[2] != NULL)
// 	{
// 		ft_putstr_fd("Error, exit, too many arguments\n", STDERR_FILENO);
// 		return (FAIL);
// 	}
// 	exit_status = ft_atoi(args[1]); // 문자열을 숫자로 변환
// 	exit(exit_status % 256); // 종료 상태 코드가 0~255 범위에 있도록 처리
// }


int mini_exit(t_Command *command)
{
    int exit_status;
    size_t i = 0;

    ft_putstr_fd("exit\n", 2);
    if (command->args[1] == NULL)
    {
        command->exitcode = 0;  // 종료 코드 0으로 설정
        exit(command->exitcode);
    }
    if (command->args[2] != NULL)
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        command->exitcode = 1; // 잘못된 인자 수로 인한 오류
        return (command->exitcode);
    }
    while (command->args[1][i] != '\0')
    {
        if (command->args[1][i] == '-' || command->args[1][i] == '+')
            i++;
        if (!ft_isdigit(command->args[1][i]))
            print_exit_err_msg(command->args[1]);
        i++;
    }
    exit_status = ft_atoi(command->args[1]);  // 종료 코드 설정
    command->exitcode = exit_status % 256; // 종료 코드 범위 제한
    if (command->exitcode < 0)
        command->exitcode += 256;
    ft_putnbr_fd(command->exitcode, 2);
    exit(command->exitcode);
}
