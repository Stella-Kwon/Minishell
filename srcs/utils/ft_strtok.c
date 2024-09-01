/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:50:26 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 19:19:59 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// 현재 문자가 구분자인지 확인하는 함수
static int is_delimiter(char ch, const char *delimiters)
{
    while (*delimiters)
    {
        if (ch == *delimiters)
            return TRUE;
        delimiters++;
    }
    return FALSE;
}

// 다음 토큰의 시작 위치를 찾는 함수
static char *find_next_token_start(char *start, const char *delimiters)
{
    // 같이 도는것을 확인해서 둘다 맞는게 있으면 넘어가줘야해.
    // 즉, delimiter가 연달아 일어나면 계속 움직여줘 포인터 다음 문자 나올떄까지 옮겨주기.
    while (*start && is_delimiter(*start, delimiters))
        start++;
    return start;
}

char *ft_strtok(char **input, const char *delimiters)
{
    static char *static_start;
    char *token_start;
    char *result;

    result = NULL;
    // 새로운 문자열 시작 시 static_start 설정
    // 그렇기에 while하고 콜백함수에서 input을  NULL로 설정해줘야해
    if (input != NULL && *input != NULL) // or로 하면 동시에 돼서 segmentation fault : &&로 input이 NULL이 아닌지를 먼저 확인하고, 그 다음에 *input을 체크
        static_start = *input;
    // 아무 것도 처리할 문자열이 없으면 NULL 반환
    if (static_start == NULL)
        return NULL;
    // 현재 토큰의 시작 위치 설정
    token_start = static_start;
    while (*static_start)
    {
        // 먼저 (' ' || '\t' || '\n' || '\v' || '\f' || '\r') 일 경우 모두 \space로 취급하는거 바꿔주기
        if (*static_start == ' ' || *static_start == '\t' || *static_start == '\n' || *static_start == '\v' || *static_start == '\f' || *static_start == '\r')
            *static_start = ' ';
        if (*static_start == 31)
        {
            static_start++;
            token_start = static_start;
            while (*static_start == 31)
                static_start++;
            printf("static_start  1 : %s\n", static_start);
            while (*static_start != 31)
                static_start++;
            printf("static_start  2 : %s\n", static_start);
            *static_start = '\0';
            static_start++; // 다음으로 포인터 넘겨주기
            result = ft_strdup(token_start);
            if (!result)
                log_errors("ft_Strdup error in ft_strtok");
            return (result);
        }
        if (*static_start == '|')
        {
            static_start++;
            return (ft_strdup("|"));
        }
        if (is_delimiter(*static_start, delimiters))
        {
            *static_start = '\0'; // 구분자를 '\0'로 변경하여 토큰 종료
            static_start = find_next_token_start(static_start + 1, delimiters);
            result = ft_strdup(token_start);
            if (!result)
                log_errors("ft_Strdup error in ft_strtok");
            return (result); // 유효한 토큰 반환
        }
        static_start++;
    }
    // 마지막 토큰 처리 : 이렇게 처리해야 static_start가 끝났다는 의미.
    if (*token_start == '\0')
        return NULL;     // 토큰이 없으면 NULL 반환
    static_start = NULL; // 모든 토큰이 처리된 경우
    return (result);
}
