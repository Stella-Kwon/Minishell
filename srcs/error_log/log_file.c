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

int	handle_error(int custom_error_code)
{
	int	exit_status;

	if (custom_error_code != 0)
		exit_status = custom_error_code;
	else if (errno == EACCES)
		exit_status = 126;
	else if (errno == ENOENT)
		exit_status = 127;
	else
		exit_status = 1;

	if (exit_status < 0 || exit_status > 255)
		exit_status = 255;

	return (exit_status);
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
