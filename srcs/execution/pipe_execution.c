/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 21:18:15 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int pipenode_exec(t_ASTNode **node)
// {
//     int wstatus;

//     // NULL 체크
//     if (node == NULL || *node == NULL)
//         return (log_errors("pipenode_exec: AST node is NULL", ""));

//     // 파이프 생성
//     if (pipe((*node)->pipeline->fd) == PIPE_FAILURE)
//         return (log_errors("MINISHELL: Failed to create pipe", strerror(errno)));

//     // 왼쪽 명령어 실행
//     if ((*node)->left)
//     {
//         (*node)->pipeline->left_pid = fork();
//         if ((*node)->pipeline->left_pid == FORK_FAILURE)
//             return (log_errors("PIPE: Failed to fork for left command", ""));
//         if ((*node)->pipeline->left_pid == CHILD)
//         {
//             close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
//             if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) == FAIL)
//                 exit(EXIT_FAILURE);
//             // 왼쪽 노드의 명령어 실행
//             ast_node_execution(&(*node)->left);
//             exit(EXIT_SUCCESS); // 자식 프로세스 종료
//         }
//     }

//     // 오른쪽 명령어 실행
//     if ((*node)->right)
//     {
//         (*node)->pipeline->right_pid = fork();
//         if ((*node)->pipeline->right_pid == FORK_FAILURE)
//             return (log_errors("PIPE: Failed to fork for right command", ""));
//         if ((*node)->pipeline->right_pid == CHILD)
//         {
//             close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기
//             if (dup_and_close((*node)->pipeline->fd[0], STDIN_FILENO) == FAIL)
//                 exit(EXIT_FAILURE);
//             // 오른쪽 노드의 명령어 실행
//             ast_node_execution(&(*node)->right);
//             exit(EXIT_SUCCESS); // 자식 프로세스 종료
//         }
//     }

//     // 부모 프로세스 - 자식 프로세스가 종료된 이후에 파이프 닫기
//     close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
//     close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기

//     waitpid((*node)->pipeline->left_pid, &wstatus, 0);
//     (*node)->last_exitcode = WEXITSTATUS(wstatus); // 왼쪽 명령어의 종료 코드 기록

//     waitpid((*node)->pipeline->right_pid, &wstatus, 0);
//     (*node)->last_exitcode = WEXITSTATUS(wstatus); // 오른쪽 명령어의 종료 코드 기록

//     return SUCCESS;
// }


// #include "../../includes/minishell.h"

int pipenode_exec(t_ASTNode **node)
{
    int wstatus;

    // NULL 체크
    if (node == NULL || *node == NULL)
        return (log_errors("pipenode_exec: AST node is NULL", ""));

    // 파이프 생성
    if (pipe((*node)->pipeline->fd) == PIPE_FAILURE)
        return (log_errors("MINISHELL: Failed to create pipe", strerror(errno)));

    // 왼쪽 명령어 실행
    if ((*node)->left)
    {
        (*node)->pipeline->left_pid = fork();
        if ((*node)->pipeline->left_pid == FORK_FAILURE)
            return (log_errors("PIPE: Failed to fork for left command", ""));
        if ((*node)->pipeline->left_pid == CHILD)
        {
            close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
            if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) == FAIL)
                exit(EXIT_FAILURE);
            // 왼쪽 노드의 명령어 실행
            ast_node_execution(&(*node)->left);
            exit(EXIT_SUCCESS); // 자식 프로세스 종료
        }
    }

    // 오른쪽 명령어 실행
    if ((*node)->right)
    {
        (*node)->pipeline->right_pid = fork();
        if ((*node)->pipeline->right_pid == FORK_FAILURE)
            return (log_errors("PIPE: Failed to fork for right command", ""));
        if ((*node)->pipeline->right_pid == CHILD)
        {
            close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기
            if (dup_and_close((*node)->pipeline->fd[0], STDIN_FILENO) == FAIL)
                exit(EXIT_FAILURE);
            // 오른쪽 노드의 명령어 실행
            ast_node_execution(&(*node)->right);
            exit(EXIT_SUCCESS); // 자식 프로세스 종료
        }
    }

    // 부모 프로세스 - 자식 프로세스가 종료된 이후에 파이프 닫기
    close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
    close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기

    // 왼쪽 명령어의 종료 코드 기록
    waitpid((*node)->pipeline->left_pid, &wstatus, 0);
    (*node)->last_exitcode = waitpid_status(wstatus); 

    // 오른쪽 명령어의 종료 코드 기록
    waitpid((*node)->pipeline->right_pid, &wstatus, 0);
    (*node)->last_exitcode = waitpid_status(wstatus); 

    return SUCCESS;
}
