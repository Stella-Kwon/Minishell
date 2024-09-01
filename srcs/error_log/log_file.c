/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 19:01:04 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//time 나중에 제출시 취소

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

int    log_errors(char *msg)
{
    int fd;

    fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("minishell.log open failed");
        return (handle_error(0));
    }
    // 현재 시간 얻기
    time_t now = time(NULL);
    if (now == (time_t)-1) {
        perror("Failed to get current time");
        close(fd);
        return (handle_error(0));
    }
    // 현재 시간을 문자열로 변환
    char *timestamp = ctime(&now);
    if (timestamp == NULL)
    {
        perror("Failed to convert time to string : ");
        close(fd);
        return (handle_error(0));
    }
    // 문자열의 끝에 있는 개행 문자 제거
    timestamp[ft_strlen(timestamp) - 1] = '\0';
    
    // 표준 에러에 오류 메시지 출력
    ft_putstr_fd("bash: ", 2);
    perror(msg);

    // 로그 파일에 시간과 오류 메시지 기록
    ft_putstr_fd(timestamp, fd);
    ft_putstr_fd(" : ", fd);
    ft_putstr_fd(strerror(errno), fd);
    ft_putstr_fd("\n", fd); // 줄바꿈 추가

    close(fd);
    return (handle_error(0));
}