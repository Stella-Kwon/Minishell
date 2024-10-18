/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 22:11:48 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 23:10:28 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_charjoin(char **line, t_line *readline)
{
	char	*join;

	join = NULL;
	(*line)[readline->i] = readline->c;
	readline->r_byte = read(STDIN_FILENO, &readline->c, 1);
	if (readline->r_byte == -1)
		return (FAIL);
	readline->i++;
	if (readline->i == readline->buf_size)
	{
		readline->buf_size *= 2;
		join = (char *)ft_calloc(readline->buf_size, 1);
		if (!(join))
			return (FAIL);
		ft_strlcpy(join, *line, readline->buf_size);
		free(*line);
		*line = join;
	}
	return (SUCCESS);
}

int	handle_eof(char **line, t_line *readline)
{
	if (readline->i == 0)
	{
		free_one((void **)line);
		return (FAIL);
	}
	return (SUCCESS);
}

int	read_byte(char **line, t_line *readline)
{
	readline->r_byte = read(STDIN_FILENO, &readline->c, 1);
	if (readline->r_byte == -1)
	{
		if (errno == EINTR)
		{
			free_one((void **)line);
			g_received_signal = 130;
			return (3);
		}
		free_one((void **)line);
		return (FAIL);
	}
	return (SUCCESS);
}

int	read_input(char **line, t_line *readline, int *exit)
{
	while (1)
	{
		*exit = read_byte(line, readline);
		if (*exit == FAIL)
			return (FAIL);
		if (*exit == 3)
			return (3);
		if (readline->r_byte == 0)
		{
			if (handle_eof(line, readline) == FAIL)
				return (3);
			break ;
		}
		if (readline->c == '\n')
			break ;
		if (ft_charjoin(line, readline) == -1)
		{
			free_one((void **)line);
			return (FAIL);
		}
		ft_putstr_fd("> ", 1);
	}
	return (SUCCESS);
}

int	read_line(char **line)
{
	t_line	readline;
	int		exit;

	readline.buf_size = 50;
	*line = (char *)calloc(readline.buf_size, 1);
	if (!(*line))
		return (FAIL);
	readline.i = 0;
	ft_putstr_fd("> ", 1);
	exit = read_input(line, &readline, &exit);
	if (exit == FAIL)
		return (FAIL);
	if (exit == 3)
		return (3);
	return (SUCCESS);
}
