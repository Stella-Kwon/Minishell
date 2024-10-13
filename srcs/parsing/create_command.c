/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:40:24 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/13 19:29:59 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int parsing(char ***tmp_args, t_Redirection **redirect, int start)
{
	int i;

	i = 0;
	while (**tmp_args)
	{
		i = redirection_parsing(tmp_args, redirect, start);
		if (i == FAIL)
			return (FAIL);
		if (i == 2)
			return (SUCCESS);
	}
	return (SUCCESS);
}

int parsing_others(char ***args, t_Redirection **redirect, int start)
{
	int i;
	char **tmp_args;

	i = 0;
	if (!args || !*args || !**args)
		return (SUCCESS);
	if (start == FALSE)
	{
		tmp_args = *args;
		while (tmp_args && *tmp_args && is_redirection(tmp_args) == FALSE)
			tmp_args++;
		if (!tmp_args || !*tmp_args)
			return (SUCCESS);
		i = parsing(&tmp_args, redirect, start);
		if (i == FAIL)
			return (FAIL);
	}
	else
	{
		i = parsing(args, redirect, start);
		if (i == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

static int command_initialize(t_Command *res, char ***tokens,
							  int buffersize, char ***env)
{
	res->cmd = ft_strdup(**tokens);
	if (!res->cmd)
	{
		log_errors("Failed to malloc res->cmd in create_command", "");
		free_command(&res);
		return (FAIL);
	}
	res->env = env;
	res->args = (char **)malloc(sizeof(char *) * buffersize);
	if (!res->args)
	{
		log_errors("Failed to malloc res->args in create_command", "");
		free_command(&res);
		return (FAIL);
	}
	return (SUCCESS);
}

static int create_command_args(t_Command *res, char ***tokens,
							   int *buffersize, int *args_index)
{
	while (**tokens && !is_operator(*tokens))
	{
		res->args[*args_index] = ft_strdup(**tokens);
		if (!res->args[*args_index])
		{
			log_errors("Failed to malloc res->args[args_index] \
			in create_command",
					   "");
			free_command(&res);
			return (FAIL);
		}
		(*tokens)++;
		(*args_index)++;
		res->args = ft_realloc_double(res->args, *args_index, buffersize);
		if (!res->args)
		{
			log_errors("Failed to realloc res->args in create_command", "");
			free_command(&res);
			return (FAIL);
		}
	}
	res->args[*args_index] = NULL;
	return (SUCCESS);
}

t_Command *create_command(char ***tokens, char ***env)
{
	t_Command *res;
	int buffersize;
	int args_index;

	buffersize = BUFFER_SIZE;
	args_index = 0;
	if (!tokens || !*tokens)
		return (NULL);
	res = (t_Command *)malloc(sizeof(t_Command));
	if (!res)
	{
		log_errors("Failed to malloc res in create_command", "");
		return (NULL);
	}
	if (command_initialize(res, tokens, buffersize, env) == FAIL)
		return (NULL);
	if (create_command_args(res, tokens, &buffersize, &args_index) == FAIL)
		return (NULL);
	res->exitcode = -1;
	res->wstatus = 0;
	return (res);
}
