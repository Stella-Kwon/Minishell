/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:56:53 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int pipe_action_parents(Command *cmd, Pipeline *pipeline)
{
    waitpid(pipeline->pid, &cmd->wstatus, 0); 
    close(pipeline->fd[1]); 
    if (dup2(pipeline->fd[0], STDIN_FILENO) == -1)
        return (log_errors("Failed to dup2 fd[0] in pipe_action_parents", ""));
    close(pipeline->fd[0]);
    if (dup2(pipeline->tmp_fd, STDIN_FILENO) == -1)
        return (log_errors("Failed to dup2 tmp_fd in pipe_action_parents", ""));
    close(pipeline->tmp_fd);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}

int pipe_execute_command(ASTNode **node)
{
    int pid;

    pid = fork();
    if (pid == -1)
        return (log_errors("Failed to fork in execute_command", ""));
    if (pid == 0)
	{
		if (action_child((*node)->command, (*node)->redir, (*node)->pipeline) != SUCCESS)
			exit ((*node)->command->exitcode);
	}
    else
    {
		(*node)->pipeline->pid = pid;
        if (pipe_action_parents((*node)->command, (*node)->pipeline) != SUCCESS)
            return (*node)->command->exitcode;
    }
    return (SUCCESS);
}

int PIPEnode_exec(ASTNode **node)
{
    if ((*node)->left) // pipe용 따로 해줘야함.ㅎ 왜냐면 구현할때 parents부분이 달라지기때문에.
    {
        if (pipe_execute_command(&(*node)->left) != SUCCESS)
            return (FAIL);
    }
    if ((*node)->right)
    {
        if (ast_node_execution(&(*node)->right) != SUCCESS)
            return (FAIL);
    }
    return (SUCCESS);
}
