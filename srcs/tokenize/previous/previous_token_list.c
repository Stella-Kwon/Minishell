/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:29:06 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/14 19:16:07 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

token_malloc *create_token_st()
{
    token_malloc *token_st;
    
    token_st = (token_malloc *)malloc(sizeof(token_malloc));
    if (!token_st)
    {
        log_errors("token_st failed to allocate memory in create_token_st()");
        return (NULL);
    }
    token_st->bufsize = 50;
    token_st->position = 0;
    token_st->in_single_quote = 0;
    token_st->in_double_quote = 0;
    token_st->in_left_braket = 0;
    token_st->in_right_braket = 0;
    token_st->new_token_list = NULL;
    token_st->start = NULL;
    token_st->token_list = (char **)malloc(token_st->bufsize * sizeof(char *));
    if (!token_list)
    {
        log_errors("Fail to malloc token_list");
        return (NULL);
    }
    return (token_st);
}

token_malloc *remalloc_tokenlist(token_malloc **token_st)//bufsize떄매 &로 보내주는데 사실 나중에도 써먹을거면
{
    // 이전 토큰들의 보존을 위해 새로운곳에 다시 크기 조정해서 멜록
    (*token_st)->new_token_list = ft_realloc(token_list, (*token_st)->position, &((*token_st)->bufsize));
    if (!(*token_st)->new_token_list)
    {
        all_free(&((*token_st)->token_list));
        return (NULL);
    }
    (*token_st)->token_list = (*token_st)->new_token_list;
    all_free((*token_st)->new_token_list);
    return ((*token_st)->token_list);
}


char **token_list(char *input)
{
    token_malloc *token_st;

    token_st = create_token_st();
    if (!token_st)
    {
        log_errors("Failed to allocate token_st");
        return (NULL);
    }
    token_st->start = input;
    while (*(token_st->start))
    {
        //여기서 조건을 따로 쪼개야한다.
        // token_st->token_list[token_st->position]
        token_st->position++;
        token_st->token_list = remalloc_tokenlist(&token_st);
        if(!token_st->token_list)
        {
            log_errors("Failed to re_allocate token_st");
            return (NULL);
        }
        
    }
    token_st->token_list[token_st->position] = NULL;
    return (token_st->token_list);
}

void free_tokenlist(char **token_list)
{
    if (token_list)
        all_free(&token_list);
}

void free_tokens(char **token_list, char **input_new, char *input)
{
    if (token_list)
        all_free(&token_list);
    if (input)
        free(input);
    if (input_new)
        all_free(&input_new);

    // int i = 0;
    // while (input[i])
    // {
    //     printf("token free: %s\n", input[i]);
    //     free(input[i]);
    //     i++;
    // }
    // i = 0;
    // while (token_list[i])
    // {
    //     printf("token free: %s\n", token_list[i]);
    //     free(token_list[i]);
    //     i++;
    // }
    // free(token_list);
}

//ft_strtok 과 같이 썼던.
// char **token_list(char **input)
// {
//     int bufsize;
//     int position;
//     char **new_token_list;
//     char **token_list;
//     char *token;

//     bufsize = 50;
//     position = 0;
//     *input = is_it_set_quote(input);
//     printf("set after input : %s\n", *input);
//     if (!*input)
//     {
//         log_errors("Fail to check is_it_set_quote");
//         return (NULL);
//     }
//     token_list = (char **)malloc(bufsize * sizeof(char *));
//     if (!token_list)
//     {
//         log_errors("Fail to malloc token_list");
//         free(*input);
//         return (NULL);
//         // exit or return;
//     }
//     token = ft_strtok(input, " "); // 여기서 \t \v \f등등 나중에 넣겠음
//     while (token)
//     {
//         token_list[position] = token;
//         position++;
//         if (position + 1 >= bufsize)
//         {
//             // 이전 토큰들의 보존을 위해 새로운곳에 다시 맬록
//             new_token_list = ft_realloc(token_list, position, &bufsize);
//             if (!new_token_list)
//             {
//                 all_free(&token_list);
//                 return (NULL);
//             }
//             token_list = new_token_list;
//         }
//         token = ft_strtok(NULL, " "); // 이렇게 넣어줘야 static이 유지;
//     }
//     token_list[position] = NULL;
//     return (token_list);
// }
