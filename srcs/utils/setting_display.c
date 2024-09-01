/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:51:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 14:06:39 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int set_ehcoctl(int flag)
{
    struct termios attr;

    // 현재 터미널 속성 가져오기 // input관련해서
    if (tcgetattr(STDIN_FILENO, &attr) == -1)
        return (log_errors("tcgetattr"));
    //  ECHO 설정
    //     term.c_lflag |= ECHO; // 입력된 문자가 화면에 표시됨
    //     term.c_lflag &= ~ECHO; // 입력된 문자가 화면에 표시되지 않음

    // ECHOCTL 설정
    if (!flag)
        attr.c_lflag |= ECHOCTL; // 제어 문자가 화면에 표시됨  ^\  ^C이런거
    else
        attr.c_lflag &= ~ECHOCTL; // 제어 문자가 화면에 표시되지 않음
        
    // 새로운 속성 적용  :  TCSANOW =  당장적용
    if (tcsetattr(STDIN_FILENO, TCSANOW, &attr) == -1)
        return (log_errors("tcsetattr"));
    return (SUCCESS);
}

