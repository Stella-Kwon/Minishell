
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:17:10 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:34 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    set_ref(char *start, char *ref)
{
    int i;

    i = 0;
    *ref = '\0';
    while (start[i]) 
    {
        if (start[i] == '"') 
        {
            *ref = '"';
            break;
        } 
        else if (start[i] == '\'')
        {
            *ref = '\'';
            break;
        }
        i++;
    }
}

void set_ref_and_tmp_start(char *start, t_Set *set, char *ref)
{
    set_ref(start, ref);
    while (*set->tmp_start)
    {
        update_quotes_and_depth(&set->single_quote, &set->double_quote,
                                &set->depth, *set->tmp_start);
        set->tmp_start++;
    }
    set->tmp_start = start;
}


char *store_words(t_For_tokenize *tokenize)
{
    t_Set set;
    char ref;
    char *start;
    
    start = tokenize->start;
    initialize_set(tokenize->start, &set);
    while (*tokenize->start && !ft_isspace(*tokenize->start) && *tokenize->start != '(' &&
           *tokenize->start != ')' && *tokenize->start != '|' && *tokenize->start != '&' &&
           *tokenize->start != '>' && *tokenize->start != '<')
    {
        if (*tokenize->start == '"' || *tokenize->start == '\'')
        {
            set_ref_and_tmp_start(tokenize->start, &set, &ref);
            if (check_quotes_and_depth(tokenize, &set, ref) == SUCCESS)
            {
                tokenize->start = set.tmp_end;
                break;
            }
        }
        (tokenize->start)++;
    }
    return (ft_strndup(start, tokenize->start - start));
}

int store_str(t_For_tokenize *tokenize)
{

    if (!ft_isspace(*tokenize->start) && *tokenize->start != '\0')
    {
        tokenize->tokens[tokenize->token_count] =
            store_words(tokenize);
        if (!tokenize->tokens[tokenize->token_count])
        {
            all_free(&tokenize->tokens);
            return (log_errors("Failed to store word", ""));
        }
        tokenize->token_count++;
        tokenize->tokens = ft_realloc_double(tokenize->tokens, \
        tokenize->token_count, &tokenize->buffsize);
        if (!tokenize->tokens)
            return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
    }
    return (SUCCESS);
}