/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:20:17 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/21 17:02:47 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int here_string(Command *command) // sending string through pipe
{
    command->infile = open(".herestring.tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (command->infile == -1)
        return (log_errors("Failed to open file in here_string"));
    if (write(command->infile, command->herestring_str, ft_strlen(command->herestring_str)) == -1)
        return (log_errors("Failed to write in here_string"));
    close(command->infile);
    command->infile = open(".heredoc.tmp", O_RDONLY);
    if (command->infile == -1)
        return (log_errors("Failed to open file in here_string"));
    if (unlink(".herestring.tmp") == -1) // unlink로 파일을 시스템에서 삭제 (이미 열린 FD에는 영향 없음)
        return (log_errors("Failed to unlink in here_string"));
    return (SUCCESS);
}
