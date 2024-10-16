/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 03:54:19 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/16 10:54:50 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_extra_args(char **args, int *argc, int i, int start)
{
	int	j;

	j = i + 1;
	while (j < *argc)
	{
		args[start + 1 + j - (i + 1)] = args[j];
		j++;
	}
	*argc -= (i - start);
	j = *argc;
	while (j < start + (i - start) + 1)
	{
		args[j] = NULL;
		j++;
	}
}

static void	fill_merged(char **args, int start, int i, char *merged)
{
	int	j;

	j = start;
	while (j <= i)
	{
		ft_strcat(merged, args[j]);
		if (j < i)
			ft_strcat(merged, " ");
		j++;
	}
	j = start - 1;
	while (j++ <= i)
		free(args[j]);
}

static int	join_args(char **args, int i, int start)
{
	int		j;
	size_t	length;
	char	*merged;

	length = 0;
	j = start;
	while (j <= i)
	{
		length += ft_strlen(args[j]) + 1;
		j++;
	}
	merged = malloc(sizeof(char) * length);
	if (!merged)
	{
		log_errors("Failed malloc in merge", "");
		return (FAIL);
	}
	merged[0] = '\0';
	fill_merged(args, start, i, merged);
	args[start] = merged;
	return (SUCCESS);
}

static char	*find_start_quote(char **args, int i, char *quote_char)
{
	char	*start_quote;

	start_quote = NULL;
	*quote_char = '\0';
	start_quote = ft_strchr(args[i], '"');
	if (start_quote != NULL)
		*quote_char = '"';
	else
	{
		start_quote = ft_strchr(args[i], '\'');
		if (start_quote != NULL)
			*quote_char = '\'';
	}
	return (start_quote);
}

void	merge_quoted_args(char **args, int *argc)
{
	int		i;
	int		start;
	char	*start_quote;
	char	*end_quote;
	char	quote_char;

	i = 0;
	while (i < *argc)
	{
		start_quote = find_start_quote(args, i, &quote_char);
		if (start_quote != NULL)
		{
			start = i;
			end_quote = ft_strchr(start_quote + 1, quote_char);
			while (end_quote == NULL && i + 1 < *argc)
				end_quote = ft_strchr(args[i++], quote_char);
			if (end_quote != NULL)
			{
				if (join_args(args, i, start) == FAIL)
					return ;
				remove_extra_args(args, argc, i, start);
			}
		}
		i++;
	}
}
