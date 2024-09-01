/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:10:56 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/01 23:34:16 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv) //(int argc, char **argv, char **env)
{
    char **tokenlist;
    char *input;
    Command *node;

    node = initialize_command();
    tokenlist = NULL;
    (void)tokenlist;
    (void)argv;
    if (argc != 1)
        return (1);
    init_signal();//얘는 부모에서만. 자식이 있을시 글로벌 변수로 소통해줘야함.
    set_ehcoctl(1);
    while ((input = readline("MINISHELL > ")) != NULL) // NULL인경우는 ctrl-D를 받을 경우
    {
        if (input[0] == '\0')
        {
            // 입력이 없는 경우 : enter쳤을 경우로 생각하면됌.
            free(input);
            continue; //루프로 다시시작.
        }
        add_history(input);
        tokenlist = token_list(&input);
        if (!tokenlist)
        {
            node->exitcode = handle_error(0);
            free(input);
            continue;
        }
        free(input);
        free_tokens(tokenlist);
    }
    if (!input) // ctrl + d라고생각하면됌
    {
        printf("\n");
        rl_replace_line("", 0); 
        rl_redisplay();
        ft_putstr_fd(" exit", 2);
        return (FAIL);
    }
    return (node->exitcode);
}
