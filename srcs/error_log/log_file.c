/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 18:42:53 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_258_exitcode_print(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	return (2);
}

// void	handle_error(t_Command **command, char *path)
// {
// 	(*command)->exitcode = errno;
// 	// printf("errno :%d\n", errno);
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(path, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	if (errno == EACCES)
// 	{
// 		// ft_putstr_fd("Permission denied\n", STDERR_FILENO);
// 		ft_putstr_fd("is a directory1\n", STDERR_FILENO);
// 		(*command)->exitcode = 126;
// 	}
// 	else if (errno == ENOENT)
// 	{
// 		ft_putstr_fd("No /such file or directory\n", STDERR_FILENO);
// 		(*command)->exitcode = 127;
// 	}
// 	else if (errno == EISDIR) // 디렉토리일 경우
// 	{
// 		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
// 		(*command)->exitcode = 126;
// 	}
// 	else
// 		(*command)->exitcode = 1;
// 	// printf("(*command)->exitcode : %d\n", (*command)->exitcode);
// }


//""
void	handle_error(t_Command **command, char *path)
{
	int	len;

	len = ft_strlen(path);
	(*command)->exitcode = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if(path[len -1] == '/')
	{
		ft_putstr_fd("Is a directory", STDERR_FILENO);
		(*command)->exitcode = 126;
		return ;
	}
	if (errno == EACCES)
	{
		// 디렉터리일 경우를 정확히 처리
		if (access(path, F_OK) == 0 && access(path, X_OK) == -1)
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		else
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		(*command)->exitcode = 126;
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		(*command)->exitcode = 127;
	}
	else if (errno == EISDIR)
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		(*command)->exitcode = 126;
	}
	else
	{
		ft_putstr_fd("Unknown error\n", STDERR_FILENO);
		(*command)->exitcode = 1;
	}
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
