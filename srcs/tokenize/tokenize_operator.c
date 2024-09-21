/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_operator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:09:30 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/18 18:34:00 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_pipe_and_or(char **tokens, char **start, int *token_count)
{
    int len;

    if (*(*start + 1) == '|')
        len = 2;
    else
        len = 1;
    return (handle_token(tokens, start, token_count, len));
}

int handle_And_and_background(char **tokens, char **start, int *token_count)
{
    int len;

    if (*(*start + 1) == '&')
        len = 2;
    else
        len = 1;
    return (handle_token(tokens, start, token_count, len));
}

int handle_input_redirection(char **tokens, char **start, int *token_count)
{
    int len;

    if (*(*start + 1) == '<') // check the next char is <
    {
        if (*(*start + 2) == '<')
            len = 3; // herestring (<<<)
        else
            len = 2; // here-doc (<<)
    }
    else
        len = 1; // redirection (<)
    return (handle_token(tokens, start, token_count, len));
}

int handle_output_redirection(char **tokens, char **start, int *token_count)
{
    int len;

    if (*(*start + 1) == '>')
        len = 2; // extension (>>)
    else
        len = 1; // redirection (>)
    return (handle_token(tokens, start, token_count, len));
}

int handle_token(char **tokens, char **start, int *token_count, int len)
{
    tokens[*token_count] = ft_strndup(*start, len);
    if (!tokens[*token_count])
    {
        all_free(tokens);
        log_errors("Failed to ft_strndup in tokenize_input");
        return (0); // 실패 0 반환
    }
    (*token_count)++;
    *start += len;
    return (1); // 성공 1 반환
}

int handle_set(char **tokens, char **start, int *token_count, char c)
{
    tokens[*token_count] = check_set(*start, start, tokens, *token_count, c);
    if (!tokens[*token_count])
    {
        all_free(tokens);
        log_errors("Failed to handle set token in tokenize_input");
        return (0); // 실패 0 반환
    }
    (*token_count)++;
    return (1); // 성공 1 반환
}