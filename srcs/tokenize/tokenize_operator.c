/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:09:30 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/03 16:15:54 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_pipe_and_or(char **input, char **tokens, char **start, int *token_count)
{
    int len;
    // char *tmp_start;
    int i;

    i = 0;
    if (*(*start + 1) == '|')
        len = 2;
    else
        len = 1;
    // tmp_start = *start + len;
    while (ft_isspace(*(*start + (++i))))
        ;
    if (*(*start + len + i - 1) == '\0')
    {
        if (check_operation_next(input, start, 0) == FAIL)
            return (FAIL);
    }
    return (handle_token(tokens, start, token_count, len));
}

int handle_And_and_background(char **input, char **tokens, char **start, int *token_count)
{
    int len;
    int i;

    i = 0;
    if (*(*start + 1) == '&')
        len = 2;
    else
        len = 1;
    while (ft_isspace(*(*start + (++i))))
        ;
    if (*(*start + len + i) == '\0')
    {

        if (check_operation_next(input, start, 0) == FAIL)
        {
            return (FAIL);
        }

    }
    // printf("changed input : %s\n", *input);
    return (handle_token(tokens, start, token_count, len));
}

int redirect_operation_error(char *start) // 시작점이 <<면 마지막위치 다음문자에 있어야된다.
{
    // printf("start in redir error: %c\n", *start);

    while (ft_isspace(*start))
        (start)++;
    if (*(start) == '\0')
        return (handle_258_exitcode_print("newline"));
    // printf("start in redir error 1: %c\n", *start);
    if (*(start) == '|')
    {
        if (*(start + 1) == '|')
            return (handle_258_exitcode_print("'||'"));
        else
            return (handle_258_exitcode_print("'|'"));
    }
    else if (*(start) == '&')
    {
        if (*(start + 1) == '&')
            return (handle_258_exitcode_print("'&&'"));
        else
            return (handle_258_exitcode_print("'&'"));
    }
    else if (*(start) == '<')
    {
        if (*(start + 1) == '<')
            return (handle_258_exitcode_print("'<<'"));
        else if (*(start + 2) == '<')
            return (handle_258_exitcode_print("'<<<'"));
        else
            return (handle_258_exitcode_print("'<'"));
    }
    else if (*(start) == '>')
    {
        if (*(start + 1) == '>')
            return (handle_258_exitcode_print("'>>'"));
        else
            return (handle_258_exitcode_print("'>'"));
    }
    return (SUCCESS);
}

int handle_input_redirection(char **tokens, char **start, int *token_count)
{
    int len;
    int i;

    i = 1;
    while (ft_isspace(*(*start + i)))
        i++;
    if (!*(*start + i))
        return (handle_258_exitcode_print("newline"));
    if (*(*start + 1) == '&') // 원래 다른 로직이 있음. 파일디스크립터 복사. 근데 현재는 하지말자.
    {
        // start + 2로 주게되면 이상한 곳을 가르키기 떄문에 &(*(start) + 2) 주려했지만
        // 저렇게는 컴파일할때 받아주지않아서
        // 어차피 start 위치는 ft_isspace는 밑에 token 저장할때 넘겨주면되고,
        // + 1 같이 +가 붙게되면 복사해 새로운 주소가 생성돼 전달되는것이라
        // 원래의 start는 주소변경 없기때문에 **start로 인자를 넣어줄필요없어서
        // *로 바꿔서 *(start) + 1로 전달.
        if (!*(*(start) + 2))
            return (handle_258_exitcode_print("newline"));
        else if (redirect_operation_error(*(start) + 2) == FAIL)
            return (FAIL);
    }
    if (*(*start + 1) == '<') // heredoc
    {
        if (*(*start + 2) == '<')
        {
            len = 3; // herestring (<<<)
            if (redirect_operation_error(*(start) + 3) == FAIL)
                return (FAIL);
        }
        else
        {
            len = 2; // here-doc (<<)
            // printf("heredoc : %s\n", *(start) + 2);
            if (redirect_operation_error(*(start) + 2) == FAIL)
                return (FAIL);
        }
    }
    else
        len = 1; // redirection (<)
    return (handle_token(tokens, start, token_count, len));
}

int handle_output_redirection(char **tokens, char **start, int *token_count)
{
    int len;
    int i;

    i = 1;
    while (ft_isspace(*(*start + i)))
        i++;
    if (!*(*start + i))
        return (handle_258_exitcode_print("newline"));
    if (*(*start + 1) == '&') // 원래 다른 로직이 있음. 파일디스크립터 복사. 근데 현재는 하지말자.
    {
        if (!*(*(start) + 2))
            return (handle_258_exitcode_print("newline"));
        else if (redirect_operation_error(*(start) + 2) == FAIL)
            return (FAIL);
    }
    if (*(*start + 1) == '>')
    {
        len = 2; // extension (>>)
        if (redirect_operation_error(*(start) + 2) == FAIL)
            return (FAIL);
    }
    else
    {
        len = 1; // redirection (>)
        if (redirect_operation_error(*(start) + 1) == FAIL)
            return (FAIL);
    }
    return (handle_token(tokens, start, token_count, len));
}

int handle_token(char **tokens, char **start, int *token_count, int len)
{
    // printf("handle token : %s\n", *start);
    while (ft_isspace(**start))
        (*start)++;
    // printf("handle token : %s\n", *start);
    tokens[*token_count] = ft_strndup(*start, len);
    // printf("token[%i] : %s\n", *token_count, tokens[*token_count]);
    if (!tokens[*token_count])
    {
        all_free(&tokens);
        log_errors("Failed to ft_strndup in tokenize_input", "");
        return (FAIL);
    }
    (*token_count)++;
    *start += len;
    return (SUCCESS);
}

int handle_set(char **tokens, char **start, int *token_count, char **input, char ref)
{
    if (**start == ')')
        return (handle_258_exitcode_print(")"));
    tokens[*token_count] = check_set(input, start, tokens, token_count, ref);
    if (!tokens[*token_count])
    {
        all_free(&tokens);
        log_errors("Failed to handle set token in tokenize_input", "");
        return (FAIL);
    }
    (*token_count)++;
    if (**start == '(')
    {
        while (ft_isspace(**start))
            (*start)++;
        if (!**start)
            return (handle_258_exitcode_print(")"));
    }
    return (SUCCESS);
}