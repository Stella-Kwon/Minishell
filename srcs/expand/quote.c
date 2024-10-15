/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 01:55:19 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/15 03:24:01 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_quote(char *input, t_Dollar *dol)
{
	dol->i++;
	while (input[dol->i] && input[dol->i] != '\'')
	{
		dol->output[dol->out_i++] = input[dol->i++];
		if (dol->out_i >= dol->out_len - 1)
		{
			dol->output = ft_realloc_single(dol->output, dol->out_i, \
											(int *)&dol->out_len);
			if (!dol->output)
			{
				log_errors("Failed realloc in quote", "");
				return (FAIL);
			}
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
			dol->output[dol->out_i++] = input[dol->i++];
			if (dol->out_i >= dol->out_len - 1)
			{
				dol->output = ft_realloc_single(dol->output, dol->out_i, \
												(int *)&dol->out_len);
				if (!dol->output)
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
