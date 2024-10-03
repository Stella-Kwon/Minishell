/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:04:06 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 05:57:41 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//일단 input이 없을때 signal 핸들링이랑...
//실행중 중단되는 경우의 핸들링일때 어떻게 해결해야할지...
// 그리고 현재 글로번 변수를 어떻게 exit넘버로 넘겨줄건지 다시 고민.
int g_received_signal = 0; // 초기화

void signal_no_input(int signal)
{
    if (signal == SIGINT)
    {
        printf("\n");
        rl_on_new_line();       // readline의 내부 상태를 새로운 줄로 업데이트
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        g_received_signal = 1;  // exitcode 신호 전달
        // printf("g_received_signal : %d\n", g_received_signal);
    }
}

void signal_execution_process(int signal)
{
    if (signal == SIGINT)
    { // ctrl + c
        printf("\n");
        rl_replace_line("", 0);  // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();          // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^C\n", 2); // 메시지 출력
        g_received_signal = 130;
    }
    else if (signal == SIGQUIT)
    { // (ctrl + \)
        printf("\n");
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^\\Quit: 3\n", 2);
        g_received_signal = 131;
    }
}

void init_signal()
{
    struct sigaction sa;
    // 핸들러 함수 설정
    sa.sa_handler = signal_no_input;
    // 비어 있는 신호 집합으로 설정 (블록할 신호 없음)
    // 핸들러가 호출될 때 블록할 신호가 없도록 비어 있는 신호 집합을 설정합니다.
    // 이로 인해 핸들러가 실행되는 동안 다른 신호가 수신되지 않도록 합니다.
    // 블록할 신호 설정
    sigemptyset(&sa.sa_mask);       // 초기화
    //예)
    // sigaddset(&sa.sa_mask, SIGINT); // SIGINT를 블록할 신호로 추가
    // sigaddset(&sa.sa_mask, SIGQUIT); // SIGINT를 블록할 신호로 추가 

    // 플래그 초기화
    sa.sa_flags = 0; // 기본값 사용
    // sa.sa_flags = SA_RESTART; // 시스템 호출이 중단되지 않고 재시작되도록 설정
    // SIGINT 및 SIGQUIT에 대해 sigaction 설정
    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("sigaction SIGINT");
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        perror("sigaction SIGQUIT");
}


void init_execution_signal()
{
    struct sigaction sa;

    sa.sa_handler = signal_execution_process;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // SIGINT 및 SIGQUIT 핸들러 설정
    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("sigaction SIGINT");
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
        perror("sigaction SIGQUIT");
}
