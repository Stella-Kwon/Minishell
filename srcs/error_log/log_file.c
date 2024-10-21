/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 10:12:26 by skwon2           ###   ########.fr       */
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

int	check_specific_error(char *path)
{
	if (errno == EACCES)
	{
		if (access(path, F_OK) == 0 && access(path, X_OK) == -1)
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		else
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		return (126);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	else if (errno == EISDIR)
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		return (126);
	}
	else
	{
		ft_putstr_fd("Unknown error\n", STDERR_FILENO);
		return (1);
	}
}

int	handle_error(char *path)
{
	int	len;

	len = ft_strlen(path);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (path[len -1] == '/')
	{
		ft_putstr_fd("Is a directory", STDERR_FILENO);
		return (126);
	}
	return (check_specific_error(path));
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
