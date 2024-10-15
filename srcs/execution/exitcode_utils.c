/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exitcode_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 15:37:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 01:10:09 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void set_last_exitcode(t_ASTNode **node, int last_exitcode)
{
    (*node)->last_exitcode = last_exitcode;
    if ((*node)->left)
        set_last_exitcode(&(*node)->left, last_exitcode);
    if ((*node)->right)
        set_last_exitcode(&(*node)->right, last_exitcode);
}

static void read_exitcode(t_ASTNode **node, int *exitcode)
{
    if (*node)
    {
        if ((*node)->left)
            read_exitcode(&(*node)->left, exitcode);
        if ((*node)->right)
            read_exitcode(&(*node)->right, exitcode);
        if ((*node)->type == NODE_COMMAND && !(*node)->command)
        {
            *exitcode = 0;
        }
        else if ((*node)->command && (*node)->command->exitcode != -1) // 커맨드노드일때
        {
            *exitcode = (*node)->command->exitcode;
        }
        else if ((*node)->type == NODE_PIPE) // 결국 맨위로 돌아온 파이프의 경우 받아오는 마자막 error코드
        {
            *exitcode = (*node)->last_exitcode;
        }
    }
}

void get_last_exitcode(t_ASTNode **node, int *last_exitcode)
{
    int exitcode;

    exitcode = -1;
    read_exitcode(node, &exitcode);
    *last_exitcode = exitcode;
}
