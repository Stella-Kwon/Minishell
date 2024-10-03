/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 20:59:50 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:00:05 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//이건 첫번째로 들어온 인풋이 operation으로 시작할때 경우 에러 프린트 .
int check_first_input(char **start)
{
    while (ft_isspace(*(*start)))
        (*start)++;
    // 입력이 파이프 또는 논리 연산자로 시작하는지 확인
    if (**start == '|')
    {
        if (*(*start + 1) == '|') // "||" 처리
            handle_258_exitcode_print("'||'");
        else // "|" 처리
            handle_258_exitcode_print("'|'");
    }
    else if (**start == '&')
    {
        if (*(*start + 1) == '&') // "&&" 처리
            handle_258_exitcode_print("'&&'");
        else // "&" 처리
            handle_258_exitcode_print("'&'");
    }
    else
        return (SUCCESS); // 첫시작이 연산자가 아닐떄.
    return (FAIL);
}

// "||", "&&", "|" 이후에 아무것도 없으면 readline 호출 근데 &일때는 flag를 사용해서 전달.
int check_operation_next(char **input, char **start, int flag)
{
    char *new_input;
    ptrdiff_t offset; // ptrdiff_t로 포인터 차이 출력을 위해선 이 데이타 타입.
    char *st; 
    char *in;

    st = *start; // start가 가리키는 값을 저장 (포인터)
    in = *input; // input이 가리키는 값을 저장 (포인터)
    offset = st - in;                   // 두 포인터 간의 차이 계산
    // printf("offset : %td\n", offset);   // ptrdiff_t로 포인터 차이 출력
    new_input = readline("> ");
    if (!new_input)
    {
        log_errors("Failed to readline for additional input", "");
        return (FAIL);
    }
    if (flag)
        add_history(new_input);
    // 입력을 기존 input과 합치기
    char *tmp = ft_strjoin(" ", new_input);
    free(new_input);
    if (!tmp)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }

    // 새로운 input과 기존 input을 합침
    new_input = ft_strjoin(*input, tmp);
    
    free(tmp);
    if (!new_input)
    {
        log_errors("Failed to join inputs", "");
        return (FAIL);
    }
    free(*input);
    *input = new_input;
    *start = *input + offset;
    // printf("start : %s\n", *start);
    add_history(*input);
    return (SUCCESS);
}
