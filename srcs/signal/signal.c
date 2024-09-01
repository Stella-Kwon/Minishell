/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 19:42:28 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//일단 input이 없을때 signal 핸들링이랑...
//실행중 중단되는 경우의 핸들링일때 어떻게 해결해야할지...
// 그리고 현재 글로번 변수를 어떻게 exit넘버로 넘겨줄건지 다시 고민.

void signal_no_input(int signal)
{
    g_received_signal = signal;//일단 여기다 설정해놓고 main가서 이 신호에 대한 값 설정.
    if (signal == SIGINT)
    {
        printf("\n");
        rl_on_new_line(); // 이렇게 안하면 minishell> 안나옴
        // 명시적으로 어떤 특정 동작을 실행하고  Readline의 내부 상태를 새로운 줄로 업데이트
        // **rl_on_new_line();
        // **을 호출하여 Readline이 새로운 줄로 이동하도록 합니다.하지만 화면에서 새로운 줄을 실제로 추가하지는 않습니다.따라서 실제 화면에 새로운 줄을 추가하려면 printf("\n");
        // 를 호출해야 합니다.
        rl_replace_line("", 0);  // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();          // 화면을 업데이트하고 빈 줄을 표시
    }
}

void signal_parents_process(int signal)
{
    g_received_signal = signal;
    if (signal == SIGINT) //ctrl + c
    {
        printf("\n");
        rl_replace_line("", 0);  // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();          // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^C\n", 2); // 이부분이랑 밑의 부분은 추후에 다시 구조짜서 정리해보기. waitpid랑 함께
    }
    if (signal == SIGQUIT) // ctrl + \
    {
        printf("\n");
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^\\Quit: 3\n", 2);
}

int init_signal(input)
{
    signal(SIGINT, signal_no_input); 
    signal(SIGQUIT, signal_no_input);

    if (g_received_signal = SIGINT)
        return (1);
    return (0);
}

int init_parent_signal(input)
{
    signal(SIGINT, signal_parents_process);
    signal(SIGQUIT, signal_parents_process);

    if (g_received_signal = SIGINT)
        return (130);
    else if (g_received_signal = SIGQUIT)
        return (131);
    return (0);
}

