/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:01:05 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int readline_again(char **input, t_set *set, char **start)
{
    char    *new_input;
    char    *new_start;
    char    *result;
    char    *tmp;
    ptrdiff_t   offset;
    char *st;
    char *in;

    while (set->single_quote || set->double_quote || set->depth > 0)
    {
        st = *start;                        // start가 가리키는 값을 저장 (포인터)
        in = *input;                        // input이 가리키는 값을 저장 (포인터) //이렇게 안해주고 바로 *start - *input하면 이상한 값나옴
        // printf("start : %p\n", (void *)st); // 포인터 출력
        // printf("input : %p\n", (void *)in); // 포인터 출력
        offset = st - in;                   // 두 포인터 간의 차이 계산
        // printf("offset : %td\n", offset);   // ptrdiff_t로 포인터 차이 출력
        new_input = readline("> ");
        if (!new_input)
            return (log_errors("new_input readline(>) failed", ""));
        tmp = ft_strjoin("\n", new_input);
        free(new_input); // new_input 메모리 해제
        if (!tmp)
            return (log_errors("Fail to add new line before add readline", ""));
        result = ft_strjoin(*input, tmp);
        free(tmp); // tmp 메모리 해제
        if (!result)
            return (log_errors("Fail to strjoin in \"readline_again\"", ""));
        new_start = result + ft_strlen(*input); // 새로 추가된 부분부터 탐색
        while (*new_start)
        {
            update_quotes_and_depth(&set->single_quote, &set->double_quote, &set->depth, *new_start);

            if (!set->single_quote && !set->double_quote && set->depth == 0)
                break; // 모든 세트가 닫히면 루프 종료

            new_start++; // 마지막 "')될때는 ++돼서 안가기때문에
        }
        free(*input);
        *input = result; // 업데이트된 문자열을 다시 input으로
        // printf("input : %s\n", *input);
        *start = *input + offset; // 얜 시작점의 위치를 offset을 위해 계속 바꿔줘야하고 while룹안에. 이 거하고 콜백함수에서 *start다시 설정해줘야해
        // printf("start : %s\n", *start);
        set->tmp_start = *start;
        // printf("set->tmp_start : %s\n", set->tmp_start);
        set->tmp_end = new_start; // 마지막 "')될때는 ++돼서 안가기때문에 그냥 주기.
        // printf("set->tmp_end : %s\n", set->tmp_end);
        add_history(*input);
    }
    return (SUCCESS);
}
