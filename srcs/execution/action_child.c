/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:19:22 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int action_child(Command *cmd, Pipeline *pipeline)
{
    if (cmd->infile) // fork이후에는 여기에 있어야 자식프로세스에 반영이되지, 부모에서 불러주면 부모에만 반영된다.
    {
        if (dup2(cmd->infile, STDIN_FILENO) == -1)
            return (log_errors("Failed to dup2 in action_child"));
        close(cmd->infile);
    }
    if (cmd->herestring_str) // fork이후에는 여기에 있어야 자식프로세스에 반영이되지, 부모에서 불러주면 부모에만 반영된다.
    {
        if (here_string(cmd) != SUCCESS)
            return (FAIL);
        if (dup2(cmd->infile, STDIN_FILENO) == -1)
            return (log_errors("Failed to dup2 in action_child"));
        close(cmd->infile);
    }
    if (cmd->outfile)
    {
        if (dup2(cmd->outfile, STDOUT_FILENO) == -1)
            return (log_errors("Failed to dup2 in action_child"));
        close(cmd->outfile);
        close(pipeline->fd[0]);
        close(pipeline->fd[1]);
    }
    else
    {
        close(pipeline->fd[0]);                         // 쓰기 끝을 닫음
        if (dup2(pipeline->fd[1], STDOUT_FILENO) == -1) // 읽기 끝을 표준 입력으로 설정
            return (log_errors("Failed to dup2 fd[0] in action_child"));
        close(pipeline->fd[1]); // 읽기 끝을 닫음
    }
    execve(cmd->cmd, cmd->args, cmd->env); // 이부분 cmd->cmd는 path까지 들어가게 바꿔야댐.
    cmd->exitcode = log_errors("Execve has failed.");
    return (cmd->exitcode);
}