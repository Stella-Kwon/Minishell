/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:09:24 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/01 21:04:23 by sukwon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_charjoin(char **line, t_Line *readline)
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
		free_one((void **)line);
		*line = join;
	}
	return (SUCCESS);
}

int	read_line(char **line)
{
	t_Line	readline;

	readline.buf_size = 10;
	*line = (char *)ft_calloc(readline.buf_size, 1);
	if (!(*line))
		return (FAIL);
	readline.r_byte = read(STDIN_FILENO, &readline.c, 1);
	if (readline.r_byte == -1)
		return (FAIL);
	readline.i = 0;
	while (readline.r_byte && readline.c != '\n')
	{
		if (ft_charjoin(line, &readline) == -1)
			return (FAIL);
	}
	return (SUCCESS);
}
