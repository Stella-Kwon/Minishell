/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child_parents.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/05 22:48:55 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int common_pre_child(Redirection **redir)
{
    if ((*redir)->infile != -2) // fork이후에는 여기에 있어야 자식프로세스에 반영이되지, 부모에서 불러주면 부모에만 반영된다.
    {
        if (dup2((*redir)->infile, STDIN_FILENO) == -1)
        {
            close((*redir)->infile);
            return (log_errors("Failed to dup2 in action_child", ""));
        }
        printf("infile\n\n");
        close((*redir)->infile);
    }
    if ((*redir)->herestring_str) // fork이후에는 여기에 있어야 자식프로세스에 반영이되지, 부모에서 불러주면 부모에만 반영된다.
    {
        if (here_string(redir) != SUCCESS)
        {
            close((*redir)->infile);
            return (FAIL);
        }
        if (dup2((*redir)->infile, STDIN_FILENO) == -1)
        {
            close((*redir)->infile);
            return (log_errors("Failed to dup2 in action_child", ""));
        }
        close((*redir)->infile);
    }
    return (SUCCESS);
}

int action_child(Command **cmd, Redirection **redir, Pipeline **pipeline)
{
    (void)pipeline;
    if (common_pre_child(redir) == FAIL)
        return (FAIL);
    // printf("infile : %d\n", (*redir)->infile);
    // printf("outfile : %d\n", (*redir)->outfile);
    if ((*redir)->outfile != -2)
    {
        if (dup2((*redir)->outfile, STDOUT_FILENO) == -1)
        {
            close((*redir)->outfile);
            return (log_errors("Failed to dup2 in action_child", ""));
        }
        // printf("outfile : %d\n", (*redir)->outfile);
        close((*redir)->outfile);
    }
    // if (cmd == NULL || *cmd == NULL)
    //     return log_errors("Command is NULL", "");
    if (execute_cmd(cmd) == FAIL) // 이부분 cmd->cmd는 path까지 들어가게 바꿔야댐.
        log_errors("Execute_cmd has failed.", "");
    else
        exit(SUCCESS);
    exit ((*cmd)->exitcode);
}

int action_parents(Redirection **redir, Pipeline **pipeline)
{
    // if (pipeline == NULL || *pipeline == NULL)
    //     return log_errors("Pipeline is NULL", "");
    // if (cmd == NULL || *cmd == NULL)
    //     return log_errors("Command is NULL", "");
    // if ((*pipeline)->pid <= 0)
    //     return log_errors("Invalid PID", "");
    int wstatus;
    wstatus = -2;
    if (waitpid((*pipeline)->pid, &wstatus, 0) == -1)
        return (log_errors("waitpid failed", ""));
    // close((*pipeline)->fd[0]);
    // close((*pipeline)->fd[1]);
    // close((*pipeline)->tmp_fd);
    if ((*redir)->outfile >= 0) // outfile이 유효한 파일 디스크립터일 경우에만
        close((*redir)->outfile);
    if ((*redir)->infile >= 0) // infile이 유효한 파일 디스크립터일 경우에만
        close((*redir)->infile);
    int exitcode = waitpid_status(wstatus);
    // printf("parents\n");
    // printf("exitcode : %d\n", exitcode);
    return (exitcode);
}
