/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:09:24 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/20 17:01:43 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_charjoin_heredoc(char **line, t_Line *readline)
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

int	read_line_heredoc(char **line)
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
		if (ft_charjoin_heredoc(line, &readline) == -1)
			return (FAIL);
	}
	if (**line != '\0')
		add_history(*line);
	return (SUCCESS);
}
