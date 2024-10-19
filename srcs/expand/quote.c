/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 01:55:19 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 22:47:27 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_quote(char *input, t_Dollar *dol)
{
	char *tmp;
	dol->i++;
	while (input[dol->i] && input[dol->i] != '\'')
	{
		dol->tmp[dol->tmp_i++] = input[dol->i++];
		if (dol->tmp_i >= dol->tmp_len - 1)
		{

			tmp = ft_realloc_single(dol->tmp, dol->tmp_i, \
											(int *)&dol->tmp_len);
			if (!tmp)
			{
				log_errors("Failed realloc in quote", "");
				return (FAIL);
			}
			dol->tmp = tmp;
		}
	}
	if (input[dol->i] == '\'')
		dol->i++;
	return (SUCCESS);
}

static int	check_double_quote(char *input, t_Dollar *dol, char **env, \
								int last_exitcode)
{
	while (input[dol->i] && input[dol->i] != '\"')
	{
		if (input[dol->i] == '$')
		{
			if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL)
				return (FAIL);
		}
		else
		{
			dol->tmp[dol->tmp_i++] = input[dol->i++];
			if (dol->tmp_i >= dol->tmp_len - 1)
			{
				dol->tmp = ft_realloc_single(dol->tmp, dol->tmp_i, \
												(int *)&dol->tmp_len);
				if (!dol->tmp)
				{
					log_errors("Failed realloc in quote double", "");
					return (FAIL);
				}
			}
		}
	}
	return (SUCCESS);
}

int	handle_double_quote(char *input, t_Dollar *dol, char **env, \
						int last_exitcode)
{
	dol->i++;
	if (check_double_quote(input, dol, env, last_exitcode) == FAIL)
		return (FAIL);
	if (input[dol->i] == '\"')
		dol->i++;
	return (SUCCESS);
}

char	*remove_quotes(char *s)
{
	int		len;
	char	*new_s;

	len = ft_strlen(s);
	if ((s[0] == '"' && s[len - 1] == '"')
		|| (s[0] == '\'' && s[len - 1] == '\''))
	{
		new_s = malloc((len - 1) * sizeof(char));
		if (!new_s)
		{
			log_errors("Failed malloc in remove_quotes\n", "");
			free(s);
		}
		ft_strncpy(new_s, s + 1, len - 2);
		new_s[len - 2] = '\0';
		free(s);
		return (new_s);
	}
	return (s);
}
