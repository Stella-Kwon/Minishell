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

int pipe_action_parents(Command *cmd, Pipeline *pipeline) // 이미 콜백함수에서 노드가 &node 인 **node로 전달되기때문에 (*node)->commad로 전달해도 값이 바뀌어지게된다. &(*node)->commad 안해줘도됌
{
    // 파이프 하나로 dup2를 해주면서 리다이렉해줄때는 같은 프로세스에서 해주게되면 데이터흐름에 영향을 줄 수 있기 떄문에. 안됌.
    // 리다이렉트한 fd를 다시 복구해주면 제대로 닫히게되면서 lsof에 뜨지 않게 되기때문에
    // 부모프로세스에서 진행함. -아니였으면 다시 fork해서 핸들링했어야됐을거같지만. 그렇게 하지않게됌.
    waitpid(pipeline->pid, &cmd->wstatus, 0);
    //waitpid담에 해줘야 기존의 STDIN이 유지돼어 실행되고 자식프로세스 종류후에 변경해줘야한다.
    close(pipeline->fd[1]);                        // 쓰기 끝을 닫음
    if (dup2(pipeline->fd[0], STDIN_FILENO) == -1) // 그 파이프안의 있는 내용을 읽기 끝을 표준 입력으로 설정
        return (log_errors("Failed to dup2 fd[0] in pipe_action_parents"));
    close(pipeline->fd[0]);                         // 읽기 끝을 닫음
    if (dup2(pipeline->tmp_fd, STDIN_FILENO) == -1) // lsof에 다 닫히게 하기위해 stdin을 복구
        return (log_errors("Failed to dup2 tmp_fd in pipe_action_parents"));
    close(pipeline->tmp_fd);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}

int pipe_execute_command(ASTNode **node)
{
    int pid;

    pid = fork();
    if (pid == -1)
        return (log_errors("Failed to fork in excute_command"));
    if (pid == 0)
    {
        if (action_child((*node)->command, (*node)->pipeline) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else
    {
        if (pipe_action_parents((*node)->command, (*node)->pipeline) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    return (SUCCESS);
}

int PIPEnode_exec(ASTNode **node)
{
    if ((*node)->left) // pipe용 따로 해줘야함.ㅎ 왜냐면 구현할때 parents부분이 달라지기때문에.
    {
        if (pipe_execute_command((*node)->left) != SUCCESS)
            return (FAIL);
    }
    if ((*node)->right)
    {
        if (ast_node_execution((*node)->right) != SUCCESS)
            return (FAIL);
    }
    return (SUCCESS);
}