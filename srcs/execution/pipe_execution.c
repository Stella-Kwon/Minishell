/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 17:09:07 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int pipenode_exec(t_ASTNode **node) {
    if (pipe((*node)->pipeline->fd) == PIPE_FAILURE) // 파이프 생성
        return log_errors("PIPE: Failed to create pipe", "");

    // 왼쪽 명령어 실행
    if ((*node)->left) {
        (*node)->pipeline->left_pid = fork();
        if ((*node)->pipeline->left_pid == FORK_FAILURE)
            return log_errors("PIPE: Failed to fork for left command", "");

        if ((*node)->pipeline->left_pid == CHILD) {
            close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
            dup2((*node)->pipeline->fd[1], STDOUT_FILENO); // stdout을 파이프에 연결
            close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기

            // 왼쪽 노드의 명령어 실행
            ast_node_execution(&(*node)->left);
            exit(EXIT_SUCCESS); // 자식 프로세스 종료
        }
    }

    // 오른쪽 명령어 실행
    if ((*node)->right) {
        (*node)->pipeline->right_pid = fork();
        if ((*node)->pipeline->right_pid == FORK_FAILURE)
            return log_errors("PIPE: Failed to fork for right command", "");

        if ((*node)->pipeline->right_pid == CHILD) {
            close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기
            dup2((*node)->pipeline->fd[0], STDIN_FILENO); // stdin을 파이프에 연결
            close((*node)->pipeline->fd[0]); // 읽기 끝 닫기

            // 오른쪽 노드의 명령어 실행
            ast_node_execution(&(*node)->right);
            exit(EXIT_SUCCESS); // 자식 프로세스 종료
        }
    }

    // 부모 프로세스
    close((*node)->pipeline->fd[0]); // 파이프의 읽기 끝 닫기
    close((*node)->pipeline->fd[1]); // 파이프의 쓰기 끝 닫기

    int wstatus;
    // 왼쪽 자식 프로세스의 종료 코드 확인
    waitpid((*node)->pipeline->left_pid, &wstatus, 0);
    // int left_exit_code = WEXITSTATUS(wstatus); // 왼쪽 명령의 종료 코드

    // 오른쪽 자식 프로세스의 종료 코드 확인
    waitpid((*node)->pipeline->right_pid, &wstatus, 0);
    int right_exit_code = WEXITSTATUS(wstatus); // 오른쪽 명령의 종료 코드

    // 마지막 종료 코드는 오른쪽 명령의 종료 코드로 업데이트
    (*node)->last_exit_code = right_exit_code; 

    // 필요에 따라 왼쪽 종료 코드도 로그로 남길 수 있음
    // log_info("Left command exit code: %d", left_exit_code);
    // log_info("Right command exit code: %d", right_exit_code);

    return SUCCESS;
}
