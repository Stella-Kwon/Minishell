/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:29:06 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 18:50:35 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



char **token_list(char **input)
{
    int bufsize;
    int position;
    char **new_token_list;
    char **token_list;
    char *token;

    bufsize = 50;
    position = 0;
    *input = is_it_set_quote(input);
    printf("set after input : %s\n", *input);
    if (!*input)
    {
        log_errors("Fail to check is_it_set_quote");
        return (NULL);
    }
    token_list = (char **)malloc(bufsize * sizeof(char *));
    if (!token_list)
    {
        log_errors("Fail to malloc token_list");
        free(*input);
        return (NULL);
        // exit or return;
    }
    token = ft_strtok(input, " "); //여기서 \t \v \f등등 나중에 넣겠음
    while (token)
    {
        token_list[position] = token;
        position++;
        if (position + 1 >= bufsize)
        {
            //이전 토큰들의 보존을 위해 새로운곳에 다시 맬록
            new_token_list = ft_realloc(token_list, position, &bufsize);
            if (!new_token_list)
            {
                all_free(&token_list);
                return (NULL);
            }
            token_list = new_token_list;
        }
        token = ft_strtok(NULL, " "); // 이렇게 넣어줘야 static이 유지;
    }
    token_list[position] = NULL;
    return (token_list);
}

void free_tokens(char **token_list)
{
    int i = 0;
    while (token_list[i])
    {
        printf("token free: %s\n", token_list[i]);
        free(token_list[i]);
        i++;
    }
    free(token_list);
}
