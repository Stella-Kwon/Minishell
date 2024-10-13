/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 19:57:00 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/13 15:28:33 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_cmd(char *cmd, char **env, int last_exitcode)
{
	return (find_dollar_signs(cmd, env, last_exitcode));
}

char	**expand_args(char **args, char **env, int last_exitcode)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		args[i] = find_dollar_signs(args[i], env, last_exitcode);
		if (!args[i])
		{
			log_errors("Argument expansion failed", args[i]);
			return (NULL);
		}
		i++;
	}
	return (args);
}

int	expand_error(char *command)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": bad substition", STDERR_FILENO);
	return (FAIL);
}

static int	check_null_cmd(char *argv)
{
	int	len;
	int	i;
	int	found;

	found = 0;
	i = -1;
	len = ft_strlen(argv);
	while (++i < len)
	{
		if (argv[i] != ' ')
		{
			found = 1;
			break ;
		}
	}
	if (found == 0)
	{
		return (TRUE);
	}
	return (FALSE);
}

int	handle_empty_cmd(t_Command **command)
{
	int	i;

	while ((*command)->cmd == NULL || ft_strlen((*command)->cmd) == 0 || \
			check_null_cmd((*command)->cmd) == TRUE)
	{
		if ((*command)->args[1] == NULL)
		{
			(*command)->exitcode = 0;
			return (FAIL);
		}
		else
		{
			free((*command)->cmd);
			(*command)->cmd = (*command)->args[1];
			i = 2;
			while ((*command)->args[i] != NULL)
			{
				(*command)->args[i - 1] = (*command)->args[i];
				i++;
			}
			(*command)->args[i - 1] = NULL;
		}
	}
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
			perror("minishell: Memory allocation failed\n");
			free(s);
		}
		ft_strncpy(new_s, s + 1, len - 2);
		new_s[len - 2] = '\0';
		free(s);
		return (new_s);
	}
	return (s);
}
