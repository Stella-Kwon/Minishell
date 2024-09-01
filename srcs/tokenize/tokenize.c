/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:30:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 18:51:08 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"



int *tokenization(Command **node, char **token_list)
{

    while (token_list && *token_list) //둘다 체크해줘야해
    {
        (*node)->exitcode = check_redirection(&(*node), *token_list, *(token_list + 1));
        if ((*node)->flag == 1)
            token_list++;
        token_list++; //다음 토큰위치 변경
    }
    return ((*node)->exitcode);
}



