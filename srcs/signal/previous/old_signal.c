/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:09:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/27 00:21:55 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// 근데 여기서는 프로세스가 중단됐을시엔 ^C문자 출력돼야해.

void signal_childprocess(int signal)
{
    if (signal == SIGINT)
    {
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^C\n", 2);// 이부분이랑 밑의 부분은 추후에 다시 구조짜서 정리해보기. waitpid랑 함께
        exit(130); // 이걸 .... pipe에서 사용하는 waitpid_Status랑 같이 사용해서 할건지나중에 고민해보기
    }
    if (signal == SIGQUIT)
    {
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        ft_putstr_fd("^\\Quit: 3\n", 2);
        exit(131); //exit아니고 나중에 exitnum을 따로 저장해서 전달 받아 사용해야한다.
    }
}

void signal_parents(int signal) // 아무런 인풋을 안줬을때의 행동
{
    if (signal == SIGINT)
    {
        printf("\n");     // 이렇게 안하면 담줄 안생김
        rl_on_new_line(); // 이렇게 안하면 minishell> 안나옴
        // 명시적으로 어떤 특정 동작을 실행하고  Readline의 내부 상태를 새로운 줄로 업데이트
        // **rl_on_new_line();
        // **을 호출하여 Readline이 새로운 줄로 이동하도록 합니다.
        // 하지만 화면에서 새로운 줄을 실제로 추가하지는 않습니다.
        // 따라서 실제 화면에 새로운 줄을 추가하려면 printf("\n");를 호출해야 합니다.
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        exit(1); //이렇게 해버리면 프로그램 종료되니 exitnumber 받아갈 수 있게 만들어야댐.
    }
    if (signal == SIGQUIT)
    {
        // 아무것도 하지않아야하기ㄸㅐ문에
        rl_replace_line("", 0); // 현재 입력 줄을 빈 문자열로 교체
        rl_redisplay();         // 화면을 업데이트하고 빈 줄을 표시
        exit(0);
    }
}

// void handle_signal(int signal __attribute__((unused))) // signal을 활용 안할떄는, 인자활욜을 하지않을떄
void handle_signal(int signal)
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG); 
    // -1로 모든 자식프로세스의 상태를 기다리고. 
    // WNOHANG으로 종료되지않은 상태에서도 계속 상태를 전달받을 수 있게한다
    if (pid == -1)// 자식이 없거나 오류일떄
    {
        if (errno == ECHILD) // 굳이해줘야하나?
            signal_parents(signal);
        else
            log_errors("signal error : ");
    }
    else
        signal_childprocess(signal);
}

void heredoc_handle_signal(int signal) // maybe we can use sigaction flag to make other functions together
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG); //-1로 모든 자식프로세스의 상태를 기다리고. WNOHANG으로 종료되지않은 상태에서도 계속 상태를 전달받을 수 있게한다
    if (pid == -1)// 자식이 없거나 오류일떄
    {
        if (errno == ECHILD) // 굳이해줘야하나?
            signal_parents(signal);
        else
            log_errors("signal error : ");
    }
    else // heredoc은  Heredoc을 사용하여 데이터를 제공받는 명령어(cat, grep, sed, 등)는 자식 프로세스로 실행
        signal_parents(signal);
}

void init_signal()
{
    signal(SIGINT, handle_signal); // ctrl-c(SIGINT)해도 프로그램이 끝나지 않게 event함수를 연결
    signal(SIGQUIT, handle_signal);
}
//이 부분은 추후에 다시 논의 -  SIGTERM 관련해서도 체크하고
// exit이 맨나중의 것을 따라가야한다는것 잊지말고...