/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 19:06:47 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_special_tokens(char **tokens, char **start, int *token_count)
{
    if (**start == '\'')
        return handle_set(tokens, start, token_count, '\'');
    else if (**start == '\"')
        return handle_set(tokens, start, token_count, '\"');
    else if (**start == '(')
        return handle_set(tokens, start, token_count, '(');
    else if (**start == '|')
        return (handle_pipe_and_or(tokens, start, token_count));
    else if (**start == '&')
        return (handle_And_and_background(tokens, start, token_count));
    else if (**start == '<')
        return (handle_input_redirection(tokens, start, token_count));
    else if (**start == '>')
        return (handle_output_redirection(tokens, start, token_count));
    else if (**start == '$')
        return (handle_token(tokens, start, token_count, 1));
    else if (ft_isspace(**start))
    {
        *start += 1; // skip whitespace
        return 1;
    }
    return 0; // No token has found.
}

char **initialize_tokenization(size_t *buffsize, int *token_count)
{
    char **tokens = malloc(*buffsize * sizeof(char *));
    if (!tokens)
    {
        log_errors("Failed to allocate memory for tokens");
        return NULL;
    }
    *token_count = 0;
    return tokens;
}

int process_tokens(char **tokens, char **start, int *token_count)
{
    if (!handle_special_tokens(tokens, start, token_count))
    {
        all_free(tokens);
        log_errors("Failed to handle special tokens");
        return 0;
    }
    tokens[*token_count] = store_words(start);
    if (!tokens[*token_count])
    {
        all_free(tokens);
        log_errors("Failed to store word");
        return 0;
    }
    (*token_count)++;
    tokens = ft_realloc(tokens, token_count, BUFFSIZE);
    if (!tokens)
    {
        log_errors("Failed to \"reallocate\" memory for tokens");
        return NULL;
    }
    return 1;
}

char **tokenize_input(char *input)
{
    char *start; //postioning
    int token_count; //tokens index
    size_t buffsize;
    char **tokens;

    start = input;
    buffsize = BUFFSIZE; // Initial buffer size
    tokens = initialize_tokenization(&buffsize, &token_count);

    if (!tokens)
        return (NULL);
    while (*start)
    {
        if (!process_tokens(tokens, &start, &token_count))
            return (NULL);
    }
    tokens[token_count] = NULL; // Null-terminate the token array
    return (tokens);
}

