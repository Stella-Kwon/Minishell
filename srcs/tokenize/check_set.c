/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_set.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/18 05:15:56 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *store_inside_set(char *input, char **start, int set_flag, char *tmp_start, char *tmp_end)
{
    char *tmp;
    
    tmp = NULL;
    if (set_flag)
    {
        tmp = readline_again(input, set_flag, **start);
        if (!tmp)
        {
            log_errors("Failed to readline_again in tokenizing single_quote");
            return (NULL);
        }
    }
    else
    {
        tmp = ft_strndup(tmp_start, (tmp_end - tmp_start + 1));
        if (!tmp)
        {
            log_errors("Failed to readline_again in tokenizing single_quote");
            return (NULL);
        }
    }
    return (tmp);
}

void change_flag_and_char(int set_flag, char **start, char *tmp_start, char *c)
{
    set_flag = !set_flag; // flag for sets
    if (*c == '(')
        *c = ')';
    else    
        (*start)++;
    tmp_start = *start; // 다음 문자.
}

char *check_set(char *input, char **start, char **tokens, int token_count, char c)
{
    int set_flag;
    char *tmp_start;
    char *tmp_end;

    set_flag = 0;
    tmp_start = *start;
    tmp_end = NULL;
    if (**start == c) //나중에 25줄 넘으면 이거 안해줘도 됌. 
    {
        change_flag_and_char(set_flag, start, tmp_start, &c);
        while (**start && set_flag)
        {
            if (**start == c)
            {
                set_flag = !set_flag; // flag for sets
                // if (**start == ')') // 나중에 처리하기 힘들어서 안넣어줌...
                //     tmp_end = *start;
                // else
                tmp_end = *start - 1;
                (*start)++;
            }
            else
                (*start)++;
        }
        (tokens)[token_count] = store_inside_set(input, start, set_flag, tmp_start, tmp_end); // malloc해준 것이여서 따로 tokens 안해줘도됌.
    }
    return (tokens[token_count]);
}
