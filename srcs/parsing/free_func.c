/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:55:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 22:46:31 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_command(t_Command **res)
{
    if (*res)
    {
        if ((*res)->cmd)
            free_one((void **)&(*res)->cmd);
        if ((*res)->args)
            all_free(&(*res)->args);
        free_one((void **)&(*res));
    }
}
// void free_command(t_Command **res)
// {
//     if (res && *res) // res가 NULL이 아닌지 체크
//     {
//         if ((*res)->cmd)
//         {
//             free((*res)->cmd);  // cmd 해제
//             (*res)->cmd = NULL; // 사용 후 NULL로 설정
//         }

//         if ((*res)->args)
//         {
//             for (int i = 0; (*res)->args[i]; i++) // 각 인자 해제
//             {
//                 free((*res)->args[i]);  // 각 인자 해제
//                 (*res)->args[i] = NULL; // 사용 후 NULL로 설정
//             }
//             free((*res)->args);  // 인자 배열 해제
//             (*res)->args = NULL; // 사용 후 NULL로 설정
//         }

//         free(*res);  // 구조체 해제
//         *res = NULL; // 포인터를 NULL로 설정
//     }
// }

void free_redirection(t_Redirection **redir)
{
    if (!(*redir))
        return;
    if ((*redir)->in_filename)
        free_one((void **)&(*redir)->in_filename);
    if ((*redir)->out_filename)
        free_one((void **)&(*redir)->out_filename);
    if ((*redir)->heredoc_limiter)
        all_free(&(*redir)->heredoc_limiter);
    if ((*redir)->herestring_str)
        free_one((void **)&(*redir)->herestring_str);
    free_one((void **)&(*redir));
}

void free_astnode(t_ASTNode **node)
{
    if (*node)
    {
        if ((*node)->command)
            free_command(&(*node)->command);
        if ((*node)->pipeline)
            free_one((void **)&(*node)->pipeline);
        if ((*node)->redir)
            free_redirection(&(*node)->redir);
        if ((*node)->right)
            free_astnode(&(*node)->right);
        if ((*node)->left)
            free_astnode(&(*node)->left);
        free_one((void **)&(*node));
    }
}