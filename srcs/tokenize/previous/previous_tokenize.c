/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   previous_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:37:16 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/18 03:54:50 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/minishell.h"

// int how_many_cmds(char **token_list)
// {
//     int count;

//     count = 1;
//     while (*token_list && token_list)
//     {
//         if (*token_list == '|')
//             count ++;
//         token_list ++;
//     }
//     return (count);
// }

// int how_many_token_before_pipe(char **token_list)
// {
//     int count;

//     count = 0;
//     while (*token_list && token_list)
//     {
//         if (*token_list == '|')
//             break;
//         count++;
//         token_list++;
//     }
//     return (count);
// }

// char **pipe_list(char **token_list)//dereference 하는게 아니라서 괜찮
// {
//     static char **start;
//     int cmds_count;
//     int token_count;
//     int index;
//     char ***pipe_list;

//     start = token_list;
//     cmds_count = how_many_cmds(token_list);
//     index = cmds_count;
//     token_count = how_many_token_before_pipe(token_list);
//     pipe_list = (char ***)malloc(cmds_count * sizeof(char**));
//     while (*token_list && token_list)
//     {

//         if (*token_list == '|')
//         {
//             *token_list = '\0';

//         }
//         index++;
//     }
//     return (pipe_list);
//




//-------------ft_strtok-------------------//
// char **tokenize_input(char *input)
// {
//     char *start; //위치설정
//     char **tokens;
//     int token_count; // 토큰 index
//     int i;

//     start = input; // 위치설정
//     token_count = 0; // 토큰 index
//     i = 0;
//     tokens = malloc(BUFFSIZE * sizeof(char *));
//     if (!tokens)
//     {
//         log_errors("Failed to allocate memory for tokens");
//         return NULL;
//     }
//     while (*start)
//     {
//         // if (token_count >= BUFFSIZE)// remalloc안에 잇어서... 해줄까 고민
//         // {
//             tokens = ft_realloc(tokens, token_count, BUFFSIZE);
//             if (!tokens)
//             {
//                 log_errors("Failed to \"reallocate\" memory for tokens");
//                 return NULL;
//             }
//         // }
//         if (*start == '\'')
//         {
//             tokens[token_count] = set_check(input, &start, tokens, token_count, '\'');
//             if (!tokens[token_count])
//                 return (NULL);
//             token_count++;
//             continue;
//         }
//         else if (*start == '\"')
//         {
//             tokens[token_count] = set_check(input, &start, tokens, token_count, '\'');
//             if (!tokens[token_count])
//                 return (NULL);
//             token_count++;
//             continue;
//         }

//         else if (*start == '(' )
//         {
//             tokens[token_count] = set_check(input, &start, tokens, token_count, '(');
//             if (!tokens[token_count])
//                 return (NULL);
//             token_count++;
//             continue;
//         }
//         else if (*start == '|')
//         {
//             // Handle pipe
//             tokens[token_count] = ft_strndup(start, 1);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '|'");
//                 return NULL;
//             }
//             token_count++;
//             start++;
//             continue;
//         }
//         else if (*start == '|' && *(start + 1) == '|')
//         {
//             // Handle pipe
//             tokens[token_count] = ft_strndup(start, 2);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '||'");
//                 return NULL;
//             }
//             token_count++;
//             start++;
//             continue;
//         }
//         else if (*start == '&')
//         {
//             tokens[token_count] = ft_strndup(start, 1);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '&'");
//                 return NULL;
//             }
//             token_count++;
//             start++;
//             continue;
//         }
//         else if (*start == '&' && *(start + 1) == '&')
//         {
//             // Handle AND
//             tokens[token_count] = ft_strndup(start, 2);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '&&'");
//                 return NULL;
//             }
//             token_count++;
//             start += 2;
//             continue;
//         }
//         else if (*start == '>' && *(start + 1) == '>' && *(start + 2) == '>')
//         {
//             // Handle herestring (>>>)
//             tokens[token_count] = ft_strndup(start, 3);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '>>>'");
//                 return NULL;
//             }
//             token_count++;
//             start += 3;
//             continue;
//         }
//         else if (*start == '>' && *(start + 1) == '>')
//         {
//             // Handle heredoc (>>)
//             tokens[token_count] = ft_strndup(start, 2);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '>>'");
//                 return NULL;
//             }
//             token_count++;
//             start += 2;
//             continue;
//         }
//         else if (*start == '>')
//         {
//             // Handle redirection (>)
//             tokens[token_count] = ft_strndup(start, 1);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '>'");
//                 return NULL;
//             }
//             token_count++;
//             start++;
//             continue;
//         }
//         else if (*start == '$')
//         {
//             // Handle dollar sign
//             tokens[token_count] = ft_strndup(start, 1);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing '$'");
//                 return NULL;
//             }
//             token_count++;
//             start++;
//             continue;
//         }
//         else if (*start == ' ' || *start == '\t' || *start == '\n' \
//         || *start == '\v' || *start == '\f' || *start == '\r')
//         {
//             tokens[token_count] = ft_strndup(start, 1);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing 'space'");
//                 return NULL;
//             }
//             token_count++;
//             while (*start == ' ' || *start == '\t' || *start == '\n' \
//             || *start == '\v' || *start == '\f' || *start == '\r')
//                 start++;
//             continue;
//         }
//         else
//         {
//             // Handle other characters
//             tokens[token_count] = store_words(&start);
//             if (!tokens[token_count])
//             {
//                 all_free(tokens);
//                 log_errors("Failed to ft_strndup in tokenizing other characters");
//                 return NULL;
//             }
//             token_count++;
//             continue;
//         }
//     }
//     tokens[token_count] = NULL; // Null-terminate the token array

//     return (tokens);
// }