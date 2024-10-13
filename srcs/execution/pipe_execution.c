/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:18:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 01:14:18 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	pipenode_exec(t_ASTNode **node) 
// {
// 	int	wstatus;
// 	int	right_exit_code;

// 	right_exit_code = WEXITSTATUS(wstatus);
// 	if (pipe((*node)->pipeline->fd) == PIPE_FAILURE)
// 		return (log_errors("PIPE: Failed to create pipe", ""));
// 	if ((*node)->left)
// 	{
// 		(*node)->pipeline->left_pid = fork();
// 		if ((*node)->pipeline->left_pid == FORK_FAILURE)
// 			return (log_errors("PIPE: Failed to fork for left command", ""));
// 		if ((*node)->pipeline->left_pid == CHILD)
// 		{
// 			close((*node)->pipeline->fd[0]);
// 			dup2((*node)->pipeline->fd[1], STDOUT_FILENO);
// 			close((*node)->pipeline->fd[1]);
// 			ast_node_execution(&(*node)->left);
// 			exit(EXIT_SUCCESS);
// 		}
// 	}
// 	if ((*node)->right)
// 	{
// 		(*node)->pipeline->right_pid = fork();
// 		if ((*node)->pipeline->right_pid == FORK_FAILURE)
// 			return (log_errors("PIPE: Failed to fork for right command", ""));
// 		if ((*node)->pipeline->right_pid == CHILD)
// 		{
// 			close((*node)->pipeline->fd[1]);
// 			dup2((*node)->pipeline->fd[0], STDIN_FILENO);
// 			close((*node)->pipeline->fd[0]);
// 			ast_node_execution(&(*node)->right);
// 			exit(EXIT_SUCCESS);
// 		}
// 	}
// 	close((*node)->pipeline->fd[0]);
// 	close((*node)->pipeline->fd[1]);
// 	waitpid((*node)->pipeline->left_pid, &wstatus, 0);
// 	waitpid((*node)->pipeline->right_pid, &wstatus, 0);
// 	(*node)->last_exitcode = right_exit_code;
// 	return (SUCCESS);
// }


int pipenode_exec(t_ASTNode **node)
{
    int status;

    // 파이프 생성
    if (pipe((*node)->pipeline->fd) == -1)
        return log_errors("Failed to create pipe", strerror(errno));

    // 왼쪽 자식 프로세스 생성
    if (((*node)->pipeline->left_pid = fork()) == 0)
    {
        close((*node)->pipeline->fd[0]); // 읽기 끝 닫기
        if (dup_and_close((*node)->pipeline->fd[1], STDOUT_FILENO) != SUCCESS)
            exit(EXIT_FAILURE); // 실패 시 자식 프로세스 종료
        ast_node_execution(&(*node)->left); // 왼쪽 노드 실행
        exit(EXIT_SUCCESS);
    }

    // 오른쪽 자식 프로세스 생성
    if (((*node)->pipeline->right_pid = fork()) == 0)
    {
        close((*node)->pipeline->fd[1]); // 쓰기 끝 닫기
        if (dup_and_close((*node)->pipeline->fd[0], STDIN_FILENO) != SUCCESS)
            exit(EXIT_FAILURE); // 실패 시 자식 프로세스 종료
        ast_node_execution(&(*node)->right); // 오른쪽 노드 실행
        exit(EXIT_SUCCESS);
    }

    // 부모 프로세스에서 파이프 닫기
    close((*node)->pipeline->fd[0]);
    close((*node)->pipeline->fd[1]);

    // 자식 프로세스 종료 대기
    waitpid((*node)->pipeline->left_pid, &status, 0);
    (*node)->last_exitcode = waitpid_status(status);

    waitpid((*node)->pipeline->right_pid, &status, 0);
    (*node)->last_exitcode = waitpid_status(status);

    return (*node)->last_exitcode;
}
