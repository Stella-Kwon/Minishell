/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 15:39:07 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_258_exitcode_print(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (FAIL);
}

void	handle_error(t_Command **command, char *path)
{
	(*command)->exitcode = errno;
	// printf("errno :%d\n", errno);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (errno == EACCES)
	{
		ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		(*command)->exitcode = 126;
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("No /such file or directory\n", STDERR_FILENO);
		(*command)->exitcode = 127;
	}
	else if (errno == EISDIR) // 디렉토리일 경우
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		(*command)->exitcode = 126;
	}
	else
		(*command)->exitcode = 1;
	// printf("(*command)->exitcode : %d\n", (*command)->exitcode);
}

int	log_errors(char *token, char *msg)
{
	int	fd;

	fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("minishell.log open failed");
		return (FAIL);
	}
	ft_putstr_fd("minishell: ", 2);
	perror(token);
	ft_putstr_fd("minishell: ", fd);
	ft_putstr_fd(token, fd);
	ft_putstr_fd(": ", fd);
	ft_putstr_fd(strerror(errno), fd);
	if (msg)
	{
		ft_putstr_fd(": ", fd);
		ft_putstr_fd(msg, fd);
	}
	ft_putstr_fd("\n", fd);
	close(fd);
	return (FAIL);
}
