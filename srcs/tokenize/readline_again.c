/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_again.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:18:27 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 15:28:18 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *readline_again(char *input, int quote, char c)
{
    char *new_input;
    char *new_start;
    char *result;

    new_input = NULL;
    result = NULL;
    new_input = readline("> ");
    if (!new_input)
    {
        log_errors("new_input readline(>) failed");
        return (NULL);
    }
    new_input = ft_strjoin("\n", new_input);
    // printf("new_input : %s\n", new_input);
    if (!new_input)
    {
        // free(result);
        log_errors("Fail to add new line before add readline");
        return (NULL); // exit처리?
    }
    // free(result);
    printf("after moving input: %s\n", input);
    result = ft_strjoin(*input, new_input);
    // tmp_origin = ft_strjoin(origin_input, new_input);
    if (!result) //|| !tmp_origin)
    {
        free(new_input);
        log_errors("Fail to strjoin in \"is_it_set_quote\"");
        return (NULL); // 에러메세지 출력할까?
    }
    // add_history(tmp_origin);
    add_history(result);
    new_start = result + ft_strlen(*input);
    while (*new_start && quote)
    {
        if (*new_start == c)
            quote = !quote;
        new_start++;
    }
    free(new_input);
    input = result;
    return (input);
}

// char *is_it_set(char **input) // 이 결과 free해야돼
// {
//     int single_quote;
//     int double_quote;
//     char *new_input;
//     char *origin_input;
//     // char *tmp_origin;
//     char *new_start;
//     char *result;

//     single_quote = FALSE;
//     double_quote = FALSE;
//     new_input = NULL;
//     new_start = NULL;
//     if (!input)
//     {
//         log_errors("There is no input in \"is_it_set_quote\"");
//         return (NULL);
//     }
//     origin_input = ft_strdup(*input);
//     printf("input: %s\n", *input);
//     if (!origin_input)
//         return (NULL);
//     // tmp_input = result;
//     result = *input;
//     while (*result)
//     {
//         if (*result == '\'' && !double_quote) // !double_quote는 false라는 개념  / 그리고 " "아니고 '로 시작했으면 그사이 뭐가나오든 다 무시.
//         {
//             // 28 (FS, File Separator)
//             // 29(GS, Group Separator)
//             // 30(RS, Record Separator)
//             // 31(US, Unit Separator)
//             // 이 제어 문자는 일반적인 텍스트에서 사용되지 않아, 텍스트 처리 중 겹칠 가능성이 거의 없음
//             // 나는 " 에는 31을 쓸것.
//             single_quote = !single_quote; // 여기서는 반대로 true면 false/ false면 true로 설정하겠다는 의미
//             *result = 31;
//             // 같은 'cat '' st'되면 ''는 사라지고 "cat '' st"같이 다를경우는 표시되서 나옴.
//             // 그리고 echo cat '         sf'되면 '이후의 스페이스만 살려서 전달되고
//             // 그 외의 ' 밖의 스페이스는 아무리 많아도 1개로 취급
//         }
//         else if (*result == '\"' && !single_quote)
//         {
//             double_quote = !double_quote;
//             *result = 31;
//         }
//         (result)++;
//     }
//     while (single_quote || double_quote)
//     {
//         new_input = readline("> ");
//         if (!new_input)
//         {
//             log_errors("new_input readline(>) failed");
//             return (NULL);
//         }
//         new_input = ft_strjoin("\n", new_input);
//         // printf("new_input : %s\n", new_input);
//         if (!new_input)
//         {
//             // free(result);
//             log_errors("Fail to add new line before add readline");
//             return (NULL); // exit처리?
//         }
//         // free(result);
//         printf("after moving input: %s\n", *input);
//         result = ft_strjoin(*input, new_input);
//         // tmp_origin = ft_strjoin(origin_input, new_input);
//         if (!result )//|| !tmp_origin)
//         {
//             free(origin_input);
//             free(new_input);
//             log_errors("Fail to strjoin in \"is_it_set_quote\"");
//             return (NULL); // 에러메세지 출력할까?
//         }
//         // add_history(tmp_origin);
//         add_history(result);
//         new_start = result + ft_strlen(*input);
//         while (*new_start)
//         {
//             if (*new_start == '\'' && !double_quote)
//             {
//                 single_quote = !single_quote;
//                 *new_start = 31;
//                 // break;
//             }
//             else if (*new_start == '\"' && !single_quote)
//             {
//                 double_quote = !double_quote;
//                 *new_start = 31;
//                 // break;
//             }
//             new_start++;
//         }
//         free(new_input);
//         free(origin_input);
//         // free(tmp_origin);
//         *input = result;
//     }
//     return (*input);
// }

// // input여기서 ' " 걸러줘야지 
// // ' " 더블인지 체크 후 맞을 경우 한 묶음으로 처리
// // 그리고 아닐경우 맞는 ' 이게 들어올때까지 > 보여주면서 기다리고 결과 출력나오게.
// // 그래서 다음 줄로 넘어가 > 프롬프트가 나타나는것.
// // 이 프롬프트는 계속해서 명령어 입력을 받기 위한 대기 상태를 의미. 그렇기에 명령어를 넣어도 실행이 안됌 "ls\n"됌
// // 문자열을 순회하여 구분자를 찾음