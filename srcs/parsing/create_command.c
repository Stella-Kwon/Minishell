/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:40:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/05 22:42:07 by suminkwon        ###   ########.fr       */
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
    // printf("type : %d\n", direction_type);
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
    char *str;
    
    str = ft_strjoin(string, "\n");
    (*redirect)->herestring_str = str;
    if (!(*redirect)->herestring_str)
        return (log_errors("Failed malloc in set_herestring", ""));
    return (SUCCESS);
}

// 달러 변수 설정
// int set_dollar_vari(Redirection **redirect, char *vari)
// {
//     (*redirect)->dollar_vari = ft_strdup(vari);
//     if (!(*redirect)->dollar_vari)
//         return (log_errors("Failed malloc in set_dollar_vari", ""));
//     return (SUCCESS);
// }

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
    // else if (ft_strcmp(**args, "$") == 0)
    // {
    //     (*args)++;
    //     if (set_dollar_vari(redirect, **args) == FAIL)
    //         return (FAIL);
    //     (*args)++;
    // }
    else
    {
        
        // (*args)++;
        return (2);
    }
    return (SUCCESS);
}

int redirection_parsing(char ***args, Redirection **redirect)
{
    int direction_type;
    int i;

    i = 0;
    // printf("1 redirection_parsing :: tokens : %s\n", **args);
    if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || ft_strcmp(**args, "<") == 0)
    {
        printf("3333 redirection_parsing :: tokens : %s\n", **args);
        direction_type = get_direction_type(**args);
        if (direction_type == INVALID)
            return (log_errors("Invalid redirection type", ""));

        (*args)++;
        printf("filename :: tokens : %s && direction type :: %d\n", **args, direction_type);
        if (set_redirection(redirect, **args, direction_type) == FAIL)
            return (FAIL);
        (*args)++; // 파일명을 건너뜀
        printf("2 redirection_parsing :: tokens : %s\n", **args);
    }
    else
    {
        // printf("2 redirection_parsing :: tokens : %s\n", **args);
        i = heredoc_herestring_dollar_parsing(args, redirect);
        if (i == FAIL) 
            return (FAIL);
        else if (i == 2)
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
        printf("2  tmp_args in loop : %s\n", **tmp_args);
        if (i == FAIL)
            return (FAIL);
        if (i == 2)
            return (SUCCESS);
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
    char **t_args = *args; // 원래의 args를 저장
    while (*t_args)
    {
        // printf("cmd->t_args : %s\n", *t_args);
        t_args++; // args 포인터를 증가시킴
    }
    if (start == FALSE)
    {
        char **tmp_args = *args;
        // while (*tmp_args)
        // {
        //     printf("cmd->tmp_args : %s\n", *tmp_args);
        //     tmp_args++; // args 포인터를 증가시킴
        // }
        (tmp_args)++;
        i = parsing(&tmp_args, redirect);
        if (i == FAIL)
            return (FAIL);
        // if (i == 2)
        //     return (2);
    }
    else //맨 처음일떄.. 
    {
        // printf("args : %s\n", **args);
        i = parsing(args, redirect);
        printf("args : %s\n", **args);
        if (i == FAIL)
            return (FAIL);
        // if (i == 2)
        //     return (2);
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

        // printf("tokens in repeated : %s\n", **tokens); // 여기서 |나 다른 연산자 나와선 안돼
        res->args[args_index] = ft_strdup(**tokens);
        if (!res->args[args_index])
        {
            log_errors("Failed to malloc res->args[args_index] in create_command", "");
            free_Command(&res);
            return (NULL);
        }
        //printf("res->args : %s\n", res->args[args_index]);
        (*tokens)++;
        args_index++;
        res->args = ft_realloc(res->args, args_index, &buffersize);
        if (!res->args)
        {
            log_errors("Failed to realloc res->args in create_command", "");
            free_Command(&res);
            return (NULL);
        }
    }
    //printf("----------------------------------\n");
    res->args[args_index] = NULL;
    // 다이렉트로 res->args넣어야지char **tmp_args = res->args; 해서 넣어주면 전달이 제대로 안됨..
    res->exitcode = -1; // 기본값 설정
    res->wstatus = 0;
    return (res);
}
