/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:40:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 02:12:34 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char **tokens)
{
	if (ft_strcmp(*tokens, "&&") == 0 || ft_strcmp(*tokens, "&") == 0 || \
		ft_strcmp(*tokens, "||") == 0 || ft_strcmp(*tokens, "|") == 0 || \
		ft_strcmp(*tokens, "(") == 0)
		return (1);
	return (0);
}

int	set_redirection(t_Redirection **redirect, char *filename, \
		int direction_type)
{
	(*redirect)->filename = ft_strdup(filename);
	if (!(*redirect)->filename)
		return (log_errors("Failed in storing filename in " \
				"set_redirection", ""));
	if (direction_type == REDIRECT_OUTPUT)
		(*redirect)->outfile = open(filename, O_WRONLY | \
				O_CREAT | O_TRUNC, 0644);
	else if (direction_type == REDIRECT_APPEND)
		(*redirect)->outfile = open(filename, O_WRONLY | \
				O_CREAT | O_APPEND, 0644);
	else if (direction_type == REDIRECT_INPUT)
		(*redirect)->infile = open(filename, O_RDONLY);
	if ((*redirect)->outfile == -1 || (*redirect)->infile == -1)
		return (log_errors((*redirect)->filename, \
				"Failed in opening file in set_redirection"));
	return (SUCCESS);
}

int	set_heredoc(t_Redirection **redirect, char *limiter)
{
	(*redirect)->heredoc_limiter = ft_strdup(limiter);
	if (!(*redirect)->heredoc_limiter)
		return (log_errors("Failed malloc in set_heredoc", ""));
	return (SUCCESS);
}

int	set_herestring(t_Redirection **redirect, char *string)
{
	(*redirect)->herestring_str = ft_strdup(string);
	if (!(*redirect)->herestring_str)
		return (log_errors("Failed malloc in set_herestring", ""));
	return (SUCCESS);
}

int	set_dollar_vari(t_Redirection **redirect, char *vari)
{
	(*redirect)->dollar_vari = ft_strdup(vari);
	if (!(*redirect)->dollar_vari)
		return (log_errors("Failed malloc in set_dollar_vari", ""));
	return (SUCCESS);
}

int	get_direction_type(char *token)
{
	if (ft_strcmp(token, ">>") == 0)
		return (REDIRECT_APPEND);
	if (ft_strcmp(token, ">") == 0)
		return (REDIRECT_OUTPUT);
	if (ft_strcmp(token, "<") == 0)
		return (REDIRECT_INPUT);
	return (INVALID);
}

int	redirection_parsing(char ***args, t_Redirection **redirect)
{
	int direction_type;
	int i;

	i = 0;
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 || \
		ft_strcmp(**args, "<") == 0)
	{
		direction_type = get_direction_type(**args);
		if (direction_type == INVALID)
			return (log_errors("Invalid redirection type", ""));
		(*args)++;
		if (set_redirection(redirect, **args, direction_type) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else
	{
		i = heredoc_herestring_dollar_parsing(args, redirect);
		if (i == FAIL)
			return (FAIL);
		else if (i == 2)
			return (2);
	}
	return (SUCCESS);
}

int	heredoc_herestring_dollar_parsing(char ***args, \
		t_Redirection **redirect)
{
	if (ft_strcmp(**args, "<<") == 0)
	{
		(*args)++;
		if (set_heredoc(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else if (ft_strcmp(**args, "<<<") == 0)
	{
		(*args)++;
		if (set_herestring(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else if (ft_strcmp(**args, "$") == 0)
	{
		(*args)++;
		if (set_dollar_vari(redirect, **args) == FAIL)
			return (FAIL);
		(*args)++;
	}
	else
	{
		return (2);
	}
	return (SUCCESS);
}

int	parsing(char ***tmp_args, t_Redirection **redirect)
{
	int i;

	i = 0;
	while (**tmp_args)
	{
		i = redirection_parsing(tmp_args, redirect);
		if (i == FAIL)
			return (FAIL);
		if (i == 2)
			return (2);
	}
	return (SUCCESS);
}

int	parsing_others(char ***args, t_Redirection **redirect, int start)
{
	int		i;
	char	**tmp_args;

	tmp_args = NULL;
	i = 0;
	if (!args || !*args || !**args)
	{
		return (SUCCESS);
	}
	if (start == FALSE)
	{
		tmp_args = *args;
		(tmp_args)++;
		i = parsing(&tmp_args, redirect);
		if (i == FAIL)
			return (FAIL);
		if (i == 2)
			return (2);
	}
	else
	{
		i = parsing(args, redirect);
		if (i == FAIL)
			return (FAIL);
		if (i == 2)
			return (2);
	}
	return (SUCCESS);
}

void	free_Command(t_Command **res)
{
	if (*res)
	{
		if ((*res)->cmd)
			free_one((void **)&(*res)->cmd);
		if ((*res)->args)
			all_free(&(*res)->args);
		free_one((void**)&(*res));
	}
}

t_Command	*create_command(char ***tokens, char **env)
{
	t_Command	*res;
	int			buffersize;
	int			args_index;

	buffersize = 1;
	args_index = 0;
	if (!tokens || !*tokens)
		return (NULL);
	res = (t_Command *)malloc(sizeof(t_Command));
	if (!res)
	{
		log_errors("Failed to malloc res in create_command", "");
		return (NULL);
	}
	res->cmd = ft_strdup(**tokens);
	if (!res->cmd)
	{
		log_errors("Failed to malloc res->cmd in create_command", "");
		free_Command(&res);
		return (NULL);
	}
	res->env = env;
	res->args = (char **)malloc(sizeof(char *) * buffersize);
	if (!res->args)
	{
		log_errors("Failed to malloc res->args in create_command", "");
		free_Command(&res);
		return (NULL);
	}
	while (**tokens && !is_operator(*tokens))
	{
		res->args = ft_realloc(res->args, args_index, &buffersize);
		if (!res->args)
		{
			log_errors("Failed to realloc res->args in create_command", "");
			free_Command(&res);
			return (NULL);
		}
		res->args[args_index] = ft_strdup(**tokens);
		if (!res->args[args_index])
		{
			log_errors("Failed to malloc res->args[args_index] " \
					"in create_command", "");
			free_Command(&res);
			return (NULL);
		}
		(*tokens)++;
		args_index++;
	}
	res->args[args_index] = NULL;
	res->exitcode = -1;
	res->wstatus = 0;
	return (res);
}
