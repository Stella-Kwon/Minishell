/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 23:01:54 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int store_str(char **tokens, char **start, int *token_count, int *buffsize)
{
    if (!ft_isspace(**start) && **start != '\0') // 이걸 안하면 <다음 space가나와도 handling처리할떄 불러주는 함수가 움직여주지 않을떄가 잇어.
    {
        tokens[*token_count] = store_words(start);
        if (!tokens[*token_count])
        {
            all_free(&tokens);
            return (log_errors("Failed to store word", ""));
        }
        (*token_count)++;
        tokens = ft_realloc(tokens, *token_count, buffsize);
        if (!tokens)
            return (log_errors("Failed to \"reallocate\" memory for tokens", ""));
    }
    return (SUCCESS);
}

int handle_special_tokens(int *buffsize, char **tokens, char **start, int *token_count, char **input)
{
    // printf("start : %s\n", *start);
    if (**start == '\'')
        return handle_set(tokens, start, token_count, input, '\'');
    else if (**start == '"')
        return handle_set(tokens, start, token_count, input, '"');
    else if (**start == '(' || **start == ')')
        return handle_set(tokens, start, token_count, input, ')'); // giving ) for storing
    else if (**start == '|')
        return (handle_pipe_and_or(input, tokens, start, token_count));
    else if (**start == '&')
        return (handle_And_and_background(input, tokens, start, token_count));
    else if (**start == '<')
    {
        // printf("in : %s\n", *start);
        return (handle_input_redirection(tokens, start, token_count));
    }
    else if (**start == '>')
        return (handle_output_redirection(tokens, start, token_count));
    else if (**start == '$')
        return (handle_token(tokens, start, token_count, 1));
    else if (ft_isspace(**start))
    {
        while (ft_isspace(**start))
            (*start)++;// 무시
        // printf("space : %c\n", **start);
        return (SUCCESS);
    }
    else 
    {
        // printf("char : %c\n", **start);
        if (store_str(tokens, start, token_count, buffsize) != SUCCESS)
            return (FAIL); // 문자들 저장
    }
    return (SUCCESS); // No token has found.
}

char **initialize_tokenization(int buffsize, int *token_count)
{
    char **tokens = malloc(buffsize * sizeof(char *));
    if (!tokens)
    {
        log_errors("Failed to allocate memory for tokens", "");
        return (NULL);
    }
    *token_count = 0;
    return (tokens);
}

char **tokenize_input(char **input)
{
    char *start; //postioning
    int token_count; //tokens index
    int buffsize;
    char **tokens;

    start = *input;
    buffsize = BUFFSIZE; // Initial buffer size
    tokens = initialize_tokenization(buffsize, &token_count);
    
    if (!tokens)
        return (NULL);
    // check first input is | && ||
    if (check_first_input(&start) != SUCCESS)
        return (NULL);
    while (*start)
    {
        // printf("while : %c\n", *start);
        // if (process_tokens(tokens, &start, &token_count, &buffsize, input) != SUCCESS)
        // {
            if (handle_special_tokens(&buffsize, tokens, &start, &token_count, input) != SUCCESS)
            {
                all_free(&tokens);
                return (NULL);
            }
        // }
        // printf("after : %c\n", *start);
    }
    // printf("finish\n");
    tokens[token_count] = NULL; // Null-terminate the token array
    return (tokens);
}

