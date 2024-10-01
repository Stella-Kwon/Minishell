/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command copy.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:40:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/24 03:41:36 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_operator(char **tokens)
{
    if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 || ft_strcmp(*tokens, "||") == 0 ||
        ft_strcmp(*tokens, "|") == 0 || ft_strcmp(*tokens, "(") == 0)
        return (1);
    return (0);
}
// 리다이렉션 설정
int set_redirection(Command **res, char *filename, int direction_type)
{
    (*res)->filename = ft_strdup(filename);
    if (!(*res)->filename)
        return (log_errors("Failed in storing filename in set_redirection"));
    if (direction_type == REDIRECT_OUTPUT)
    {
        (*res)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if ((*res)->outfile == -1)
            return (log_errors("Failed in opening_outfile in set_redirection"));
    }
    else if (direction_type == REDIRECT_APPEND)
    {
        (*res)->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if ((*res)->outfile == -1)
            return (log_errors("Failed in opening_appending in set_redirection"));
    }
    else if (direction_type == REDIRECT_INPUT)
    {
        (*res)->infile = open(filename, O_RDONLY);
        if ((*res)->outfile == -1)
            return (log_errors("Failed in opening_infile in set_redirection"));
    }
    return (SUCCESS);
}

// heredoc 설정
int set_heredoc(Command **res, char *limiter)
{
    (*res)->heredoc_limiter = ft_strdup(limiter);
    if (!(*res)->heredoc_limiter)
        return (log_errors("Failed malloc in set_heredoc"));
    return (SUCCESS);
}
// herestring설정
int set_herestring(Command **res, char *string)
{
    (*res)->herestring_str = ft_strdup(string);
    if (!(*res)->herestring_str)
        return (log_errors("Failed malloc in set_herestring"));
    return (SUCCESS);
}

int set_dallor_vari(Command **res, char *vari)
{
    (*res)->dollar_vari = ft_strdup(vari);
    if (!(*res)->dollar_vari)
        return (log_errors("Failed malloc in set_dollar_vari"));
    return (SUCCESS);
}

int redirection_parsing(char ***args, Command **res)
{
    if (ft_strcmp(**args, ">") == 0)
    {
        *args++;
        if (set_redirection(*res, **args, REDIRECT_OUTPUT) == FAIL)
            return (FAIL);
        *args++; // 파일명을 건너뜀
    }
    else if (ft_strcmp(**args, ">>") == 0)
    {
        *args++;
        if (set_redirection(*res, **args, REDIRECT_OUTPUT) == FAIL)
            return (FAIL);
        *args++; // 파일명을 건너뜀
    }
    else if (ft_strcmp(**args, "<") == 0)
    {
        *args++;
        if (set_redirection(*res, **args, REDIRECT_OUTPUT) == FAIL)
            return (FAIL);
        *args++; // 파일명을 건너뜀
    }
    return (SUCCESS);
}

int    heredoc_herestring_dollar_parsing(char ***args, Command **res)
{
    if (ft_strcmp(**args, "<<") == 0)
    {
        *args++;
        if (set_heredoc(*res, **args) == FAIL)
            return (FAIL);
        *args++; // Heredoc limiter를 건너뜀
    }
    else if (ft_strcmp(**args, "<<<") == 0)
    {
        *args++;
        if (set_herestring(*res, **args) == FAIL)
            return (FAIL);
        *args++; // string을 건너뜀
    }
    else if (ft_strcmp(**args, "$") == 0)
    {
        *args++;
        if (set_dallor_vari(*res, **args) == FAIL)
            return (FAIL);
        *args++; // string을 건너뜀
    }
}

int    parsing_others(char ***args, Command **res)
{
    if (!*args || !**args)
        *args = NULL;
    while (**args)
    {
        if (redirection_parsing(args, res) == FAIL)
            return (FAIL);
        else if (heredoc_herestring_dollar_parsing(args, res) == FAIL)
            return (FAIL);
    }
    return (SUCCESS);
}

void free_Command(Command **res)
{
    if (*res)
        free(res);
    if ((*res)->cmd)
        free((*res)->cmd);
    if ((*res)->args)
        all_free((*res)->args);
    if ((*res)->heredoc_limiter)
        free((*res)->heredoc_limiter);
    if ((*res)->herestring_str)
        free((*res)->herestring_str);
    if ((*res)->filename)
        free((*res)->filename);
    if ((*res)->dollar_vari)
        free((*res)->dollar_vari);
}

Command *create_command(char **tokens, char **env)
{
    Command *res;
    int buffersize;
    int args_index;

    args_index = 0;
    buffersize = BUFFER_SIZE;
    if (*tokens)
        return (NULL);
    res = (Command *)malloc(sizeof(Command));
    if (!res)
    {
        log_errors("Failed to malloc res in create_command");
        return (NULL);
    }
    res->cmd = ft_strdup(**tokens);
    if (!res->cmd)
    {
        log_errors("Failed to malloc res->cmd in create_command");
        free_Command(&res);
        return (NULL);
    }
    // *tokens++; // command
    res->args = (char **)malloc(sizeof(char *) * BUFFER_SIZE);
    if (!res->args)
    {
        log_errors("Failed to malloc res->args in create_command");
        free_Command(&res);
        return (NULL);
    }
    while (**tokens && !is_operator(**tokens))
    
    {
        res->args[args_index] = ft_strdup(*tokens);
        if (!res->args[args_index])
        {
            log_errors("Failed to malloc res->args[args_index] in create_command");
            free_Command(&res);
            return (NULL);
        }
        *tokens++;
        res->args = ft_realloc(res->args, args_index, &buffersize);
        if (!res->args)
        {
            log_errors("Failed to realloc res->args in create_command");
            free_Command(&res);
            return (NULL);
        }
    }
    if (parsing_others(res->args, &res) == FAIL)
    {
        free_Command(&res);
        return (NULL);
    }
    // res->infile = -2;
    // res->outfile = -2;
    // res->heredoc_limiter = NULL;
    // res->herestring_str = NULL;
    // res->filename = NULL;
    // res->dollar_vari = NULL;
    res->exitcode = -1;//0이면 옳은거니까
    res->wstatus = 0;
    return (res);
}