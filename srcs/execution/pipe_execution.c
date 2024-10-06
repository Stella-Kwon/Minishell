/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:24:55 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int pipe_action_child(Command **cmd, Redirection **redir, Pipeline **pipeline)
// {
//     // 파이프의 나머지 끝과 임시 fd를 닫음 : 자식에서는 닫아도 돼 부모는 안돼
//     close((*pipeline)->fd[0]);
//     // close((*pipeline)->tmp_fd);
//     if (common_pre_child(redir) == FAIL)
//         return log_errors("in pipe action child", "");
//     // outfile이 있으면 STDOUT을 outfile로 리다이렉션
//     if ((*redir)->outfile != -2)
//     {
//         if (dup2((*redir)->outfile, STDOUT_FILENO) == -1)
//         {
//             close((*redir)->outfile);
//             return log_errors("Failed to dup2 in action_child", "");
//         }
//         close((*redir)->outfile);
//     }
//     // outfile이 없으면 파이프를 통해 STDOUT을 리다이렉션
//     else
//     {
//         printf("(*pipeline)->fd[1]  : %d\n", (*pipeline)->fd[1]);                      // 읽기 끝을 닫음
//         if (dup2((*pipeline)->fd[1], STDOUT_FILENO) == -1) // 쓰기 끝을 STDOUT으로 설정
//         {
//             close((*pipeline)->fd[1]);
//             return log_errors("Failed to dup2 fd[1] in pipe_action_child", "");
//         }
//         ft_putstr_fd("(*pipeline)->fd[1] \n\n", 2);
//         close((*pipeline)->fd[1]); // 쓰기 끝을 닫음
//     }

//     // 명령 실행
//     if (execute_cmd(cmd) == FAIL)
//         log_errors("Execute_cmd has failed in pipe_action_child.", "");
//     ft_putstr_fd("\n\nend \n\n", 2);
//     return ((*cmd)->exitcode);
// }

// int pipe_action_parents(Command **cmd, Redirection **redir, Pipeline **pipeline)
// {
//     printf("pipe parents\n");
//     // fflush(stdout);

//     // 리다이렉션 관련 파일 디스크립터를 닫음
//     // close((*pipeline)->fd[1]); // 쓰기 끝 먼저 닫기 부모가 자식이전에 하면 안돼
//     printf("(*pipeline)->fd[0]  : %d\n", (*pipeline)->fd[0]);
//     // 파이프의 읽기 끝을 STDIN으로 리다이렉션
//     if (dup2((*pipeline)->fd[0], STDIN_FILENO) == -1)
//     {
//         close((*pipeline)->fd[0]);
//         return log_errors("Failed to dup2 fd[0] in pipe_action_parents", "");
//     }
//     close((*pipeline)->fd[0]); // 사용 후 닫음

//     // 여기선 자식프로세스와 부모가 주고받는 병렬작업이라서 밑에 해줘야 실행가능.
//     if (waitpid((*pipeline)->pid, &(*cmd)->wstatus, 0) == -1)
//         return log_errors("waitpid failed", "");
//     //이후에 해주는게 좋고.
//     if ((*redir)->outfile >= 0) // outfile이 유효한 파일 디스크립터일 경우에만
//         close((*redir)->outfile);
//     if ((*redir)->infile >= 0) // infile이 유효한 파일 디스크립터일 경우에만
//         close((*redir)->infile);
//     // tmp_fd를 다시 STDIN으로 리다이렉션
//     // printf("(*pipeline)->tmp_fd  : %d\n", (*pipeline)->tmp_fd);
//     // if (dup2((*pipeline)->tmp_fd, STDIN_FILENO) == -1)
//     // {
//     //     close((*pipeline)->tmp_fd);
//     //     return log_errors("Failed to dup2 tmp_fd in pipe_action_parents", "");
//     // }
//     close((*pipeline)->fd[1]); // 사용 후 닫음
//     // close((*pipeline)->tmp_fd); // 사용 후 닫음
//     // 자식 프로세스의 종료 코드 확인
//     (*cmd)->exitcode = waitpid_status((*cmd)->wstatus);
//     printf("(*cmd)->exitcode : %d\n", (*cmd)->exitcode);

//     return ((*cmd)->exitcode);
// }

// int pipe_execute_command(t_ASTNode **node)
// {
//     int pid;

//     pid = fork();
//     if (pid == -1)
//         return (log_errors("Failed to fork in execute_command", ""));
//     if (pid == 0)
// 	{
// 		if (pipe_action_child(&(*node)->command, &(*node)->redir, &(*node)->pipeline) != SUCCESS)
// 			exit((*node)->command->exitcode);
// 	}
//     else
//     {
// 		(*node)->pipeline->pid = pid;
//         if (pipe_action_parents(&(*node)->command, &(*node)->redir, &(*node)->pipeline) != SUCCESS)
//             return (*node)->command->exitcode;
//     }
//     return (SUCCESS);
// }

// int pipe_ast_node_execution(t_ASTNode **node)
// {
//     if (node == NULL || *node == NULL)
//         return (log_errors("AST node is NULL", ""));
//     if (heredoc_check(node) == FAIL)
//         return (FAIL);
//     if ((*node)->type == NODE_COMMAND)
//     {
//         if (pipe((*node)->pipeline->fd) == -1)
//             return (log_errors("Failed to create pipe", ""));
//         printf("pipeline->fd[0]: %d, pipeline->fd[1]: %d\n", (*node)->pipeline->fd[0], (*node)->pipeline->fd[1]);
//         // (*node)->pipeline->tmp_fd = dup(STDIN_FILENO);
//         // if ((*node)->pipeline->tmp_fd == -1)
//         //     return (log_errors("Failed to backup stdin", ""));
//         return (pipe_execute_command(node));
//     }
//     return (execute_node(node));
// }

// int pipenode_exec(t_ASTNode **node)
// {
//     if ((*node)->left) // pipe용 따로 해줘야함.ㅎ 왜냐면 구현할때 parents부분이 달라지기때문에.
//     {
//         if (pipe_ast_node_execution(&(*node)->left) != SUCCESS)
//             return (FAIL);
//         // 왼쪽 노드의 자식 프로세스를 기다림
//     }
//     if ((*node)->right)
//     {
//         printf("right node in pipe \n\n");
//         (*node)->right->pipeline->tmp_fd = dup(STDIN_FILENO);
//         if ((*node)->right->pipeline->tmp_fd == -1)
//             return (log_errors("Failed to backup stdin", ""));
//         if (pipe_ast_node_execution(&(*node)->right) != SUCCESS)
//             return (FAIL);
//         printf("(*pipeline)->tmp_fd  : %d\n", (*node)->right->pipeline->tmp_fd);
//         if (dup2((*node)->right->pipeline->tmp_fd, STDIN_FILENO) == -1)
//         {
//             close((*node)->right->pipeline->tmp_fd);
//             return log_errors("Failed to dup2 tmp_fd in pipe_action_parents", "");
//         }
//         // close((*node)->right->pipeline->fd[1]);  // 사용 후 닫음
//         close((*node)->right->pipeline->tmp_fd); // 사용 후 닫음
//     }
//     return (SUCCESS);
// }

// AST 노드에서 명령어를 실행하는 함수
int execute_command(t_ASTNode **node)
{
    if (node == NULL || *node == NULL)
        return log_errors("AST node is NULL", "");

    // 자식 프로세스 생성
    (*node)->pipeline->pid = fork();
    int pid = (*node)->pipeline->pid;
    if (pid == -1)
    {
        return log_errors("Failed to fork", "");
    }

    if (pid == 0)
    { // 자식 프로세스
        // 명령어 실행
        
        ft_putstr_fd("in execution\n", 2);
        printf("(*pipeline)->fd[1]  : %d\n", (*node)->pipeline->fd[1]);
        printf("(*pipeline)->fd[0]  : %d\n", (*node)->pipeline->fd[0]);
        if (common_pre_child(&(*node)->redir) == FAIL)
            return (FAIL);
        printf("infile : %d\n", (*node)->redir->infile);
        printf("outfile : %d\n", (*node)->redir->outfile);
        if ((*node)->redir->outfile != -2)
        {
            if (dup2((*node)->redir->outfile, STDOUT_FILENO) == -1)
            {
                close((*node)->redir->outfile);
                return (log_errors("Failed to dup2 in action_child", ""));
            }
            printf("outfile : %d\n", (*node)->redir->outfile);
            close((*node)->redir->outfile);
        }
        ft_putstr_fd("command : ", 2);
        ft_putstr_fd((*node)->command->cmd, 2);
        execve((*node)->command->cmd, (*node)->command->args, (*node)->command->env);
        exit(EXIT_FAILURE); // execvp 실패 시 종료
    }
    else
    { // 부모 프로세스
        // 자식 프로세스의 종료를 기다림
        int wstatus;
        wstatus = -2;
        if (waitpid((*node)->pipeline->pid, &wstatus, 0) == -1)
            return (log_errors("waitpid failed", ""));
        if ((*node)->redir->outfile >= 0) // outfile이 유효한 파일 디스크립터일 경우에만
            close((*node)->redir->outfile);
        if ((*node)->redir->infile >= 0) // infile이 유효한 파일 디스크립터일 경우에만
            close((*node)->redir->infile);
        (*node)->command->exitcode = WEXITSTATUS(wstatus); // 종료 코드 저장
    }

    return SUCCESS; // 성공
}
// int saved_stdin;
// int saved_stdout;

// void save_standard_fds()
// {
//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);
// }

//*설명
// 같은 fd를 공유하면, 다른 프로세스 부모든 자식이든 닫혀도 상관없이 데이터 접근가능
// 일단 파이프 노드가 있으면 최 상위의 fd를 하위모든 파이프 노드에게 복사가능하게 fd를 생성해줘야 한다.
// 문제는 맨 상위 right노드에서 원래의 stdout으로 프린되도록 해야한다는 것.


int pipe_exec(t_ASTNode **node)
{
    if (node == NULL || *node == NULL)
        return log_errors("PIPE : AST node is NULL in pipe_ast_node", "");
    if (pipe((*node)->pipeline->fd) == PIPE_FAILURE)
        return log_errors("PIPE : Failed to create pipe", "");
    if ((*node)->left)
    {
        (*node)->pipeline->left_pid = fork();
        if ((*node)->pipeline->left_pid == FORK_FAILURE)
            return log_errors("PIPE : Failed to fork for left command", "");
        if ((*node)->pipeline->left_pid == CHILD)
        {
            ft_putstr_fd("leftnode\n", 2);
            // 왼쪽 명령어 실행
            printf("(*pipeline)->fd[1]  : %d\n", (*node)->pipeline->fd[1]);
            close((*node)->pipeline->fd[0]);
            dup2((*node)->pipeline->fd[1], STDOUT_FILENO); // 왼쪽 명령어의 출력을 파이프의 쓰기쪽으로
                      // 읽기쪽 파이프 닫기
            close((*node)->pipeline->fd[1]);               // 쓰기쪽 파이프 닫기
            if ((*node)->left->type == NODE_COMMAND)
            {
                if (execute_command(&(*node)->left) == FAIL) // 이부분 cmd->cmd는 path까지 들어가게 바꿔야댐.
                {
                    log_errors("PIPE : Execute_cmd has failed in left_node.", "");
                    exit((*node)->left->command->exitcode);
                }
                exit(EXIT_SUCCESS); // 성공 시 종료
            }
            ft_putstr_fd("getting in recursive\n", 2);
            if (pipe_exec(&(*node)->left) != SUCCESS)
                exit(EXIT_FAILURE); // 실패 시 종료
            exit(EXIT_SUCCESS);     // 성공 시 종료
        }
    }

    int wstatus;
    if ((*node)->pipeline->left_pid > 0)
    {
        if (waitpid((*node)->pipeline->left_pid, &wstatus, 0) == -1)
            return (log_errors("waitpid failed", ""));
    }

    if ((*node)->right)
    {
        (*node)->pipeline->right_pid = fork();
        if ((*node)->pipeline->right_pid == FORK_FAILURE)
            return log_errors("PIPE : Failed to fork for right command", "");

        if ((*node)->pipeline->right_pid == CHILD)
        {
            ft_putstr_fd("rightnode\n", 2);
            printf("(*pipeline)->fd[1]  : %d\n", (*node)->pipeline->fd[1]);
            // 오른쪽 명령어 실행
            close((*node)->pipeline->fd[1]);
            dup2((*node)->pipeline->fd[0], STDIN_FILENO); // 오른쪽 명령어의 입력을 파이프의 읽기쪽으로
                // 쓰기쪽 파이프 닫기
            close((*node)->pipeline->fd[0]);              // 읽기쪽 파이프 닫기
            if ((*node)->right->type == NODE_COMMAND)
            {
                ft_putstr_fd("getting in execution\n", 2);
                if (execute_command(&(*node)->right) == FAIL) // 이부분 cmd->cmd는 path까지 들어가게 바꿔야댐.
                {
                    log_errors("PIPE : Execute_cmd has failed in right_node.", "");
                    exit((*node)->left->command->exitcode);
                }
            }
            exit(EXIT_SUCCESS); // 성공 시 종료
        }
    }

    // 부모 프로세스에서 자식 프로세스 대기
    close((*node)->pipeline->fd[0]);
    close((*node)->pipeline->fd[1]);

    if ((*node)->pipeline->right_pid > 0)
    {
        if (waitpid((*node)->pipeline->right_pid, &wstatus, 0) == -1)
            return (log_errors("waitpid failed", ""));
    }
    return SUCCESS;
}

// // 파이프를 생성하고 AST 노드를 실행하는 함수
// int pipe_ast_node_execution(t_ASTNode **node)
// {
//     ft_putstr_fd("hello\n",2);
//     if (node == NULL || *node == NULL)
//         return log_errors("AST node is NULL", "");
//     if (heredoc_check(node) == FAIL)
//             return (FAIL);
//         // 왼쪽 자식 노드가 존재하는 경우
//     if ((*node)->left)
//     {
//         if (pipe((*node)->pipeline->fd) == -1)
//         {
//             return log_errors("Failed to create pipe", "");
//         }
//         ft_putstr_fd("(*pipeline)->fd[1] : %d\n", (*node)->pipeline->fd[1]);
//             // 왼쪽 노드의 출력이 파이프의 쓰기 쪽으로 연결됨
//         if (dup2((*node)->pipeline->fd[1], STDOUT_FILENO) == -1)
//         {
//             close((*node)->pipeline->fd[0]);
//             close((*node)->pipeline->fd[1]);
//             return log_errors("Failed to dup2 for left node", "");
//         }
//         close((*node)->pipeline->fd[1]); // 사용 후 쓰기 쪽 닫기

//         // 왼쪽 노드 실행
//         if (pipe_ast_node_execution(&(*node)->left) != 0)
//             return log_errors("Failed to pipe_ast_node_execution in left command", "");
//         if ((*node)->type == NODE_COMMAND)
//         {
//             if (execute_command(node) != SUCCESS)
//                 return log_errors("Failed to execute left command", "");
//         }
//     }

//     // 오른쪽 자식 노드가 존재하는 경우
//     if ((*node)->right)
//     {
//         // 오른쪽 노드의 입력이 파이프의 읽기 쪽으로 연결됨
//         if (dup2((*node)->pipeline->fd[0], STDIN_FILENO) == -1)
//         {
//             close((*node)->pipeline->fd[0]);
//             return log_errors("Failed to dup2 for right node", "");
//         }
//         close((*node)->pipeline->fd[0]); // 사용 후 읽기 쪽 닫기

//         // 오른쪽 노드 실행
//         if (execute_command(&(*node)->right) != SUCCESS)
//             return log_errors("Failed to execute right command", "");
//     }
//     // 표준 입출력 복원
//     if ((*node)->left)
//     {
//         // 왼쪽 노드가 있는 경우, 부모 프로세스에서 복원
//         if (dup2(saved_stdout, STDOUT_FILENO) == -1) // 올바르게 복원
//         {
//             return log_errors("Failed to restore stdout", "");
//         }
//     }

//     // 여기서 추가로 stdin 복원도 필요함
//     if ((*node)->right)
//     {
//         if (dup2(saved_stdin, STDIN_FILENO) == -1) // 올바르게 복원
//         {
//             return log_errors("Failed to restore stdin", "");
//         }
//     }

//     return 0; // 성공
// }

// 파이프 노드 실행 함수
int pipenode_exec(t_ASTNode **node)
{
    if (node && *node)
    {
        ft_putstr_fd("hello", 2);
        if (pipe_exec((node)) != 0)
            return -1;
    }
    // if ((*node)->left)
    // {
    //     // 왼쪽 노드 실행
    //     if (pipe_ast_node_execution(&(*node)->left) != 0)
    //         return -1;
    // }

    // if ((*node)->right)
    // {
    //     // 오른쪽 노드 실행
    //     if (pipe_ast_node_execution(&(*node)->right) != 0)
    //         return -1;
    // }
    // ft_putstr_fd("hello\n",2);
    return 0; // 성공
}
