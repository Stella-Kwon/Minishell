/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:59:41 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *store_inside_set(char *tmp_start, char *tmp_end)
{
    char *tmp;
    ptrdiff_t offset; // ptrdiff_t로 포인터 차이 출력을 위해선 이 데이타 타입.
    char *st;
    char *end;

    if (!tmp_start || !tmp_end || tmp_start > tmp_end)
    {
        log_errors("Invalid pointers in store_inside_set", "");
        return (NULL);
    }

    // 문자열 길이 계산 (tmp_end가 tmp_start보다 뒤에 있거나 같아야 함)
    st = tmp_start;                     // start가 가리키는 값을 저장 (포인터)
    end = tmp_end;                       // endput이 가리키는 값을 저장 (포인터)
    // printf("start : %p\n", (void *)st); // 포인터 출력
    // printf("input : %p\n", (void *)end); // 포인터 출력
    offset = end - st;                   // 두 포인터 간의 차이 계산
    // printf("offset : %td\n", offset);   // ptrdiff_t로 포인터 차이 출력 //

    // 문자열 복사
    tmp = ft_strndup(tmp_start, offset + 1); // 첫위치에서 부터 뺀거라 + 1을 해줘야 갯수에 맞음
    if (!tmp)
    {
        log_errors("Failed to allocate memory in store_inside_set", "");
        return (NULL);
    }

    return (tmp);
}

void update_quotes_and_depth(int *single_quote, int *double_quote, int *depth, char c)
{
    if (c == '\'' && !*double_quote)
        *single_quote = !*single_quote;
    else if (c == '"' && !*single_quote)
        *double_quote = !*double_quote;

    // 괄호 처리 ("" 또는 '' 안에 있을 때는 무시) // 이부분에 대해서는 오류처리함에 있어 어쩔대는 syntax에러 어쩔때는 >열어준다 ()를 맞추기위해/
    else if (!*single_quote && !*double_quote)
    {
        if (c == '(')
            (*depth)++; // 여는 괄호: 증가
        else if (c == ')')
            (*depth)--; // 닫는 괄호: 감소
    }
}

char *check_set(char **input, char **start, char **tokens, int *token_count, char ref)
{
    t_set set;

    set.depth = 0;
    set.single_quote = 0;
    set.double_quote = 0;
    set.tmp_start = *start;
    set.tmp_end = NULL;
    
    // printf("start\n");
    while (*set.tmp_start)
    {
        update_quotes_and_depth(&set.single_quote, &set.double_quote, &set.depth, *set.tmp_start);
        set.tmp_start++;
        // printf("paa\n");
    }
    set.tmp_start = *start;
    // printf("middle\n");
    if (set.depth > 0 || set.single_quote || set.double_quote)
    { // 짝이 맞지 않는 경우
        // readline 호출
        readline_again(input, &set, start);
        if (!input)
            return (NULL); // 에러 처리
        // 새로운 입력을 처리할 시작점 설정 다시 해줘야해. readline에서 한것은 -1 만큼 크기가 달라서
        *start = *input + strlen(*input); // 마지막 문자 다음으로 이동 (다음에 넘겨줘야하니까.)
    }
    else 
    {
        // printf("\n\n\nlast\n");
        set.tmp_end = *start + 1;
        while (*set.tmp_end != ref)
        {
            set.tmp_end ++;
        }
        *start = set.tmp_end + 1;
        // printf("checking start in set : %s\n", *start);
    }
    if (*set.tmp_start == '\'' || *set.tmp_start == '"')
    {
        set.tmp_start += 1;
        set.tmp_end -= 1;
    }
    if (*set.tmp_start == '(' || *set.tmp_end == ')')
    {
        tokens[*token_count] = ft_strdup("(");
        tokens[(*token_count) + 1] = ft_strdup(")");
        *token_count += 2;
        set.tmp_start += 1;
        set.tmp_end -= 1;
    }
    tokens[*token_count] = store_inside_set(set.tmp_start, set.tmp_end);
    if (!tokens[*token_count])
        return (NULL);
    return (tokens[*token_count]);
}
