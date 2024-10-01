/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:40:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 02:12:34 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// 연산자인지 확인
int is_operator(char **tokens)
{
    if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 || ft_strcmp(*tokens, "||") == 0 ||
        ft_strcmp(*tokens, "|") == 0 || ft_strcmp(*tokens, "(") == 0)
        return (1);
    return (0);
}


// 리다이렉션 설정
int set_redirection(Redirection **redirect, char *filename, int direction_type)
{
    (*redirect)->filename = ft_strdup(filename);
    if (!(*redirect)->filename)
        return (log_errors("Failed in storing filename in set_redirection", ""));

    if (direction_type == REDIRECT_OUTPUT)
        (*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (direction_type == REDIRECT_APPEND)
        (*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (direction_type == REDIRECT_INPUT)
        (*redirect)->infile = open(filename, O_RDONLY);
    // printf("outfile : %d\n infile : %d\n", (*redirect)->outfile, (*redirect)->infile);
    if ((*redirect)->outfile == -1 || (*redirect)->infile == -1)
        return (log_errors((*redirect)->filename, "Failed in opening file in set_redirection"));

    return (SUCCESS);
}

// heredoc 설정
int set_heredoc(Redirection **redirect, char *limiter)
{
    (*redirect)->heredoc_limiter = ft_strdup(limiter);
    if (!(*redirect)->heredoc_limiter)
        return (log_errors("Failed malloc in set_heredoc", ""));
    return (SUCCESS);
}

// herestring 설정
int set_herestring(Redirection **redirect, char *string)
{
    (*redirect)->herestring_str = ft_strdup(string);
    if (!(*redirect)->herestring_str)
        return (log_errors("Failed malloc in set_herestring", ""));
    return (SUCCESS);
}

// 달러 변수 설정
int set_dollar_vari(Redirection **redirect, char *vari)
{
    (*redirect)->dollar_vari = ft_strdup(vari);
    if (!(*redirect)->dollar_vari)
        return (log_errors("Failed malloc in set_dollar_vari", ""));
    return (SUCCESS);
}

// 리다이렉션 파싱
int get_direction_type(char *token)
{
    if (ft_strcmp(token, ">>") == 0)
        return (REDIRECT_APPEND);
    if (ft_strcmp(token, ">") == 0)
        return (REDIRECT_OUTPUT);
    if (ft_strcmp(token, "<") == 0)
        return (REDIRECT_INPUT);
    return (INVALID); // 유효하지 않은 방향 타입
}

int redirection_parsing(char ***args, Redirection **redirect)
{
    int direction_type;
    int i;

    i = 0;
    // printf("1 redirection_parsing :: tokens : %s\n", **args);
    if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || ft_strcmp(**args, "<") == 0)
    {
        direction_type = get_direction_type(**args);
        if (direction_type == INVALID)
            return (log_errors("Invalid redirection type", ""));

        (*args)++;
        // printf("filename :: tokens : %s && direction type :: %d\n", **args, direction_type);
        if (set_redirection(redirect, **args, direction_type) == FAIL)
            return (FAIL);
        (*args)++; // 파일명을 건너뜀
        // printf("2 redirection_parsing :: tokens : %s\n", **args);
    }
    else
    {
        i = heredoc_herestring_dollar_parsing(args, redirect);
        if (i == FAIL) 
            return (FAIL);
        else if (i == 2)
            return (2);
    }
    return (SUCCESS);
}

// heredoc, herestring, dollar 파싱
int heredoc_herestring_dollar_parsing(char ***args, Redirection **redirect)
{
    // printf("1  heredoc_herestring_dollar_parsing :: tokens : %s\n", **args);
    if (ft_strcmp(**args, "<<") == 0)
    {
        (*args)++;
        if (set_heredoc(redirect, **args) == FAIL)
            return (FAIL);
        (*args)++;
    }
    else if (ft_strcmp(**args, "<<<") == 0)
    {
        (*args)++;
        if (set_herestring(redirect, **args) == FAIL)
            return (FAIL);
        (*args)++;
    }
    else if (ft_strcmp(**args, "$") == 0)
    {
        (*args)++;
        if (set_dollar_vari(redirect, **args) == FAIL)
            return (FAIL);
        (*args)++;
    }
    else
    {
        return (2);
    }
    return (SUCCESS);
}

int parsing(char ***tmp_args, Redirection **redirect)
{
    int i;

    i = 0;
    while (**tmp_args)
    {
        // printf("1  tmp_args in loop : %s\n", **tmp_args);
        i = redirection_parsing(tmp_args, redirect);
        if (i == FAIL)
            return (FAIL);
        if (i == 2)
            return (2);
    }
    // printf("passing parsing\n\n");
    return (SUCCESS);
}


// 기타 파싱 처리
int parsing_others(char ***args, Redirection **redirect, int start) // 주어진게 args인지 tokens, 즉 맨 첫 또는 연산자 다음에 오는 첫토큰인지 아닌지에 따라서 args를 움직여주던 해야댐
{
    int i;

    i = 0;
    // printf("\n--------------GOT INTO PARSING OTEHRS----------\n\n");
    if (!args || !*args || !**args)
    {
        // printf("finish\n");
        return (SUCCESS);
    }
//  각 단계에서 NULL을 체크하여 포인터 dereferencing 시 발생할 수 있는 잠재적인 크래시 방지
// args만 체크할 수 없는 이유 : 만약 args가 NULL이 아니더라도, 그 내부의 포인터가 NULL 일 수 있음
// 이를 방지하기위해 중첩된 포인터를 체크하는 것이 안전합니다.
// 따라서, 최소한 if (!args || !*args) 는 남겨두고,
// !**args 체크는 경우에 따라 생략할 수 있지만, 안전성을 위해 모두 확인하는 것이 좋습니다.                                                                                                                                                                                                                                                                                                        
    if (start == FALSE)
    {
        char **tmp_args = *args;
        // printf("tmp_args : %s\n", *tmp_args);
        (tmp_args)++;
        i = parsing(&tmp_args, redirect);
        if (i == FAIL)
            return (FAIL);
        if (i == 2)
            return (2);
    }
    else
    {
        i = parsing(args, redirect);
        if (i == FAIL)
            return (FAIL);
        if (i == 2)
            return (2);
    }
    // printf("--------------getting in to while loopppppppp-------------\n");
    return (SUCCESS);
}

// Command 해제
void free_Command(Command **res)
{
    if (*res)
    {
        if ((*res)->cmd)
            free_one((void **)&(*res)->cmd);
        if ((*res)->args)
            all_free(&(*res)->args);
        free_one((void**)&*res);
    }
}


// Command 생성
Command *create_command(char ***tokens, char **env)
{
    Command *res;
    int buffersize = 1; //BUFFSIZE;
    int args_index = 0;

    if (!tokens || !*tokens)
        return (NULL);

    res = (Command *)malloc(sizeof(Command));
    if (!res)
    {
        log_errors("Failed to malloc res in create_command", "");
        return (NULL);
    }
    res->cmd = ft_strdup(**tokens);
    if (!res->cmd)
    {
        log_errors("Failed to malloc res->cmd in create_command", "");
        free_Command(&res);
        return (NULL);
    }
    // printf("res->cmd : %s\n", res->cmd);
    res->env = env;
    res->args = (char **)malloc(sizeof(char *) * buffersize);
    if (!res->args)
    {
        log_errors("Failed to malloc res->args in create_command", "");
        free_Command(&res);
        return (NULL);
    }
    while (**tokens && !is_operator(*tokens))
    {
        // printf("args_index : %d\n", args_index);
        res->args = ft_realloc(res->args, args_index, &buffersize);
        if (!res->args)
        {
            log_errors("Failed to realloc res->args in create_command", "");
            free_Command(&res);
            return (NULL);
        }
        // printf("tokens in repeated : %s\n", **tokens); // 여기서 |나 다른 연산자 나와선 안돼
        res->args[args_index] = ft_strdup(**tokens);
        if (!res->args[args_index])
        {
            log_errors("Failed to malloc res->args[args_index] in create_command", "");
            free_Command(&res);
            return (NULL);
        }
        printf("res->args : %s\n", res->args[args_index]);
        (*tokens)++;
        args_index++;
    }
    printf("----------------------------------\n");
    res->args[args_index] = NULL;
    // 다이렉트로 res->args넣어야지char **tmp_args = res->args; 해서 넣어주면 전달이 제대로 안됨..
    res->exitcode = -1; // 기본값 설정
    res->wstatus = 0;
    return (res);
}
