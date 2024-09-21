/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:53:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/21 17:49:22 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_check(ASTNode **node)
{
    if ((*node)->command->heredoc_limiter)
    {
        if (here_doc(node) == FAIL)
            return (log_errors("Failed here_doc in heredoc_check"));
    }
    return (SUCCESS);
}

ASTNode *ast_node_execution(ASTNode **node)
{
    if (heredoc_check(node) == FAIL)
        return (NULL);
    if (pipe((*node)->pipeline->fd) == -1)
        return (log_errors("Failed to pipe in execute_command")); // 일단 고려
    (*node)->pipeline->tmp_fd = dup(STDIN_FILENO); // defaul it as stdin_fileno.
    if ((*node)->pipeline->tmp_fd == -1)
    {
        log_errors("Failed dup in ast_node_execution");
        return (NULL);
    }
    if ((*node)->type == NODE_PIPE)
    {
        if (pipenode_exec(node) != SUCCESS)
            return ((*node)->command->exitcode);
    }
    else if ((*node)->type == NODE_OR)
    {
        
    }
    else if ((*node)->type == NODE_AND)
    {
        
    }
    else if ((*node)->type == NODE_SUBSHELL)
    {
        
    }
    else if ((*node)->type == NODE_COMMAND)
    {
        
    }
}

int pipe_action_parents(Command *cmd, Pipeline *pipeline) // 이미 콜백함수에서 노드가 &node 인 **node로 전달되기때문에 (*node)->commad로 전달해도 값이 바뀌어지게된다. &(*node)->commad 안해줘도됌
{
    //파이프 하나로 dup2를 해주면서 리다이렉해줄때는 같은 프로세스에서 해주게되면 데이터흐름에 영향을 줄 수 있기 떄문에. 안됌. 
    //리다이렉트한 fd를 다시 복구해주면 제대로 닫히게되면서 lsof에 뜨지 않게 되기때문에
    //부모프로세스에서 진행함. -아니였으면 다시 fork해서 핸들링했어야됐을거같지만. 그렇게 하지않게됌. 
    close(pipeline->fd[1]);                        // 쓰기 끝을 닫음
    if (dup2(pipeline->fd[0], STDIN_FILENO) == -1) // 읽기 끝을 표준 입력으로 설정
        return (log_errors("Failed to dup2 fd[0] in pipe_action_parents"));
    close(pipeline->fd[0]); // 읽기 끝을 닫음
    if (dup2(pipeline->tmp_fd, STDIN_FILENO) == -1)// lsof에 다 닫히게 하기위해 stdin을 복구
    return (log_errors("Failed to dup2 tmp_fd in pipe_action_parents"));
    close(pipeline->tmp_fd);
    waitpid(pipeline->pid, &cmd->wstatus, 0);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}

int action_parents(Command *cmd, Pipeline *pipeline) 
{
    waitpid(pipeline->pid, &cmd->wstatus, 0);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}

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
    execve(cmd->cmd, cmd->args, cmd->env);//이부분 cmd->cmd는 path까지 들어가게 바꿔야댐.
    cmd->exitcode = log_errors("Execve has failed.");
    return (cmd->exitcode);
}

int     pipe_execute_command(ASTNode **node)
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

int pipenode_exec(ASTNode **node)
{
    if ((*node)->left)
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