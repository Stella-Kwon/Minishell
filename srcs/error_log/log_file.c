/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/04 16:09:46 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_258_exitcode_print(char *msg)
{
    ft_putstr_fd("bash: ", 2);
    ft_putstr_fd("syntax error near unexpected token ", 2);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
    return (FAIL);
}

int handle_error(int custom_error_code)
{
    int exit_status;

    // 사용자 정의 오류 코드가 있으면 이를 우선 적용
    if (custom_error_code != 0)
    {
        exit_status = custom_error_code;
    }
    else if (errno == EACCES)
        exit_status = 126; // Permission denied
    else if (errno == ENOENT)
        exit_status = 127; // Command not found
    else
        exit_status = 1; // 일반적인 오류 (대체적인 error는 1로 bash는 취급)

    // 범위 검사는 필요 없지만, 예외적으로 255로 설정
    if (exit_status < 0 || exit_status > 255)
        exit_status = 255; // exit 상태 값이 0~255 범위를 벗어나는 경우 255로 설정

    return exit_status;
}

int    log_errors(char *token, char *msg)
{
    int fd;

    fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("minishell.log open failed");
        return (FAIL);
    }
    ft_putstr_fd("bash: ", 2);
    perror(token);

    // 로그 파일에 시간과 오류 메시지 기록
    // ft_putstr_fd(timestamp, fd);
    // ft_putstr_fd(" : ", fd);
    ft_putstr_fd("MINISHELL : ", fd);
    ft_putstr_fd(token, fd);
    ft_putstr_fd(": ", fd);
    ft_putstr_fd(strerror(errno), fd);
    ft_putstr_fd(": ", fd);
    ft_putstr_fd(msg, fd);
    ft_putstr_fd("\n", fd); // 줄바꿈 추가

    close(fd);
    return (FAIL);
}