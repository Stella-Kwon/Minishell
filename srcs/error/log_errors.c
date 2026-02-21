/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:50:33 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 20:43:52 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int log_errors(char *token, char *msg)
{
	int fd;

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
	if (msg)
	{
		ft_putstr_fd(": ", fd);
		ft_putstr_fd(msg, fd);
	}
	ft_putstr_fd("\n", fd);
	close(fd);
	return (FAIL);
}
