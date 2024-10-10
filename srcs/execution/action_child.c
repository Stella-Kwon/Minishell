/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 21:29:30 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	common_pre_child(t_Redirection	**redir)
// {
// 	if ((*redir)->infile != -2)
// 	{
// 		if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
// 			return (FAIL);
// 	}
// 	if ((*redir)->herestring_str)
// 	{
// 		if (here_string(redir) != SUCCESS)
// 			return (FAIL);
// 	if ((*redir)->infile != -2 && \
// 		dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
//             return (FAIL);
// 	}
// 	return (SUCCESS);
// }

// int	action_child(t_Command **cmd, t_Redirection **redir)
// {
// 	if (common_pre_child(redir) == FAIL)
// 		exit (FAIL);
// 	if ((*redir)->outfile != -2)
// 		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
// 			exit (FAIL);
// 	execute_cmd(cmd);
// 	exit((*cmd)->exitcode);
// }


int	common_pre_child(t_Redirection **redir)
{
    if ((*redir)->infile != -2)
    {
        if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
        {
            log_errors("Failed to redirect infile", strerror(errno)); // 에러 메시지 추가
            return (FAIL);
        }
    }

    if ((*redir)->herestring_str)
    {
        if (here_string(redir) != SUCCESS)
            return (FAIL);
        if ((*redir)->infile != -2 && \
            dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
        {
            log_errors("Failed to redirect infile after herestring", strerror(errno)); // 에러 메시지 추가
            return (FAIL);
        }
    }

    if ((*redir)->outfile != -2)
    {
        if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
        {
            log_errors("Failed to redirect outfile", strerror(errno)); // 에러 메시지 추가
            return (FAIL);
        }
    }

    return (SUCCESS);
}

int	action_child(t_Command **cmd, t_Redirection **redir)
{
    if (common_pre_child(redir) == FAIL)
    {
        log_errors("Failed in common_pre_child in child process", strerror(errno)); // 에러 처리 추가
        exit(FAIL);
    }

    if ((*redir)->outfile != -2)
    {
        if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
        {
            log_errors("Failed to redirect outfile in child process", strerror(errno)); // 에러 처리 추가
            exit(FAIL);
        }
    }

    execute_cmd(cmd); // 명령어 실행
    exit((*cmd)->exitcode); // 종료 코드 반환
}

