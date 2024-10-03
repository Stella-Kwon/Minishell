/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:57:33 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/03/14 15:17:09 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*read_and_save(int fd, char *read_buffer)
{
	char	*buffer;
	int		read_bytes;

	read_buffer = check_string(read_buffer);
	if (!read_buffer)
		return (0);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (ft_free(&read_buffer));
	read_bytes = 1;
	while (!ft_strchr(read_buffer, '\n') && read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
			return (ft_free(&read_buffer));
		buffer[read_bytes] = '\0';
		read_buffer = ft_strjoin(read_buffer, buffer);
		if (!read_buffer)
			return (ft_free(&read_buffer));
	}
	free(buffer);
	return (read_buffer);
}

char	*extract_line(char *read_buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!read_buffer[i])
		return (0);
	while (read_buffer[i] && read_buffer[i] != '\n')
		i++;
	if (read_buffer[i] != '\n')
		line = malloc(sizeof(char) * (i + 1));
	else
		line = malloc(sizeof(char) * (i + 2));
	i = 0;
	if (!line)
		return (0);
	while (read_buffer[i] && read_buffer[i] != '\n')
	{
		line[i] = read_buffer[i];
		i++;
	}
	if (read_buffer[i] && read_buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*get_remainder(char *read_buffer)
{
	char	*remainder;
	int		i;
	int		next_i;

	i = 0;
	next_i = 0;
	while (read_buffer[i] && read_buffer[i] != '\n')
		i++;
	if (read_buffer[i] == '\0')
	{
		free(read_buffer);
		return (0);
	}
	remainder = malloc(sizeof(char) * (ft_strlen(read_buffer) - i + 1));
	if (!remainder)
		return (ft_free(&read_buffer));
	i++;
	while (read_buffer[i])
		remainder[next_i++] = read_buffer[i++];
	remainder[next_i] = '\0';
	free(read_buffer);
	return (remainder);
}

char	*get_next_line(int fd)
{
	static char	*read_buffer;
	char		*curr_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read (fd, 0, 0) < 0)
		return (ft_free(&read_buffer));
	read_buffer = read_and_save(fd, read_buffer);
	if (!read_buffer)
		return (ft_free(&read_buffer));
	curr_line = extract_line(read_buffer);
	read_buffer = get_remainder(read_buffer);
	if (!curr_line || !read_buffer)
	{
		free(read_buffer);
		read_buffer = 0;
	}
	return (curr_line);
}
