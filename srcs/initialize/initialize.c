/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:43:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/29 17:02:33 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

Command *initialize_command()
{
    Command *node;

    node = (Command *)malloc(sizeof(Command));
    if (!node)
        return (NULL);
    node->cmd = NULL; // 나중에 빌트인 함수인지 확인하고 실행
    node->args = NULL;
    node->infile = -1;
    node->outfile = -1;
    node->heredoc_limiter = NULL;
    node->flag = 0;
    node->exitcode = 0;
    return node;
}