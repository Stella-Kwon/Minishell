/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parsing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/12 00:57:56 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int set_outfile(t_Redirection **redirect, char *filename, int direction_type)
{
	int file_exists;

	// 파일 존재 여부 확인
	file_exists = (access(filename, F_OK) == 0);

	if (direction_type == REDIRECT_OUTPUT)
	{
		if ((*redirect)->outfile != -2 || (*redirect)->outfile != -1)
			close((*redirect)->outfile);
		if (file_exists && filename[0] == '.' && filename[1] == '/') // 완전경로여서 그위치에 파일없으면 O_create하지말기
			(*redirect)->outfile = open(filename, O_WRONLY | O_TRUNC, 0644);

		else
			(*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		// printf("(*redirect)->outfile : %d\n", (*redirect)->outfile);
	}
	else if (direction_type == REDIRECT_APPEND)
	{
		if ((*redirect)->outfile != -2 || (*redirect)->outfile != -1)
			close((*redirect)->outfile);
		if (file_exists && filename[0] == '.' && filename[1] == '/')
			(*redirect)->outfile = open(filename, O_WRONLY | O_APPEND, 0644);
		else
			(*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		// printf("(*redirect)->outfile : %d\n", (*redirect)->outfile);
	}
	return (SUCCESS);
}

static int set_infile(t_Redirection **redirect, char ***args, char *filename, int direction_type)
{
	if (direction_type == REDIRECT_INPUT)
	{
		if (put_last_open_infile(redirect, args, &filename) != SUCCESS)
			return (FAIL);
		if ((*redirect)->heredoc_limiter != NULL)
		{
			if ((*redirect)->tmp_infile != -2)
				close((*redirect)->tmp_infile);
			(*redirect)->tmp_infile = open(filename, O_RDONLY);
		}
		if ((*redirect)->herestring_str != NULL)
			free_one((void **)&(*redirect)->herestring_str);
		// printf("filename : %s, and (*redirect)->filename : %s\n", filename, (*redirect)->filename);
		if ((*redirect)->infile != -2 || (*redirect)->infile != -1)
			close((*redirect)->infile);
		(*redirect)->infile = open(filename, O_RDONLY);
	}
	return (SUCCESS);
}

int filename_restore(int *i, char ***args, char **new_filename, int *buffersize)
{
	(void)buffersize;
	int j;

	j = 0;

	// printf("i : %d\n", *i);
	// printf("(**args)[i] : %c\n", (**args)[*i]);

	while ((**args)[*i] && *i < (int)ft_strlen(**args))
	{
		if ((**args)[*i] == '"' || (**args)[*i] == '\'')
		{
			(*i)++;
			continue;
		}
		// printf("(**args)[i] : %c\n", (**args)[*i]);
		(*new_filename)[j] = (**args)[*i];
		// printf("*new_filename[j] : %c\n", (*new_filename)[j]);
		(*i)++;
		j++;
		*new_filename = ft_realloc_single(*new_filename, j, buffersize);
		if (!new_filename)
			return (log_errors("Failed to remalloc in filename_with_quote", ""));
		if (!(**args)[*i])
			break;
	}
	(*new_filename)[j] = '\0';
	// printf("*new_filename : %sword\n", (*new_filename));
	return (SUCCESS);
}

int rm_quote_filename(t_Redirection **redirect, char ***args)
{
	char *new_filename;
	int i;
	int buffersize;

	i = 0;
	buffersize = BUFFER_SIZE;
	new_filename = NULL;
	if ((*redirect)->filename && (*redirect)->filename[0] != '"' && (*redirect)->filename[0] != '\'')
		return (SUCCESS);
	if ((**args)[i] == '"' || (**args)[i] == '\'') // 나중에 " "set맞출때는 여기다가 readline다시 열어주면됌.
	{
		i++;
		// printf("(**args)[i] : %c\n", (**args)[i]);
		// if ((**args)[i] == '.' && (**args)[i + 1] == '/')
		// 	i += 2;
		new_filename = malloc(buffersize * sizeof(char));
		if (!new_filename)
			return (log_errors("Failed to malloc new_filename in rm_quote_filename", ""));
		if (filename_restore(&i, args, &new_filename, &buffersize) != SUCCESS)
			return (log_errors("Failed to filename_restore in rm_quote_filename", ""));
		if ((*redirect)->filename)
			free_one((void **)&(*redirect)->filename);
		(*redirect)->filename = new_filename;
	}
	return (SUCCESS);
}

int set_redirection(t_Redirection **redirect, char ***args, int direction_type)
{
	char *filename;

	// printf("args in set redirection : %s\n", **args);
	if ((*redirect)->infile == -1 || (*redirect)->outfile == -1)
		return (SUCCESS);
	if ((*redirect)->infile != -1)
	{
		(*redirect)->filename = ft_strdup(**args);
		if (!(*redirect)->filename)
			return (log_errors("Failed in storing filename in set_redirection", ""));
		filename = (*redirect)->filename;
	}
	if (rm_quote_filename(redirect, args) != SUCCESS)
		return (log_errors("Failed in rm_quote_filename in set_redirection", ""));
	// printf("filename : %s, and (*redirect)->filename : %s\n", filename, (*redirect)->filename);
	if (set_outfile(redirect, filename, direction_type) != SUCCESS)
		return (FAIL);
	else if (set_infile(redirect, args, filename, direction_type) != SUCCESS)
		return (FAIL);
	// printf("fininshed setting well in set redirect");
	return (SUCCESS);
}

static int redirection_parsing_set(char ***args, t_Redirection **redirect)
{
	int direction_type;

	direction_type = get_direction_type(**args);
	if (direction_type == INVALID)
		return (log_errors("Invalid redirection type", ""));
	(*args)++;
	if (set_redirection(redirect, args, direction_type) == FAIL)
		return (FAIL);
	(*args)++;
	return (SUCCESS);
}

int redirection_parsing(char ***args, t_Redirection **redirect,
						int start)
{
	int i;

	i = 0;
	if (ft_strcmp(**args, ">") == 0 || ft_strcmp(**args, ">>") == 0 ||
		ft_strcmp(**args, "<") == 0)
	{
		if (redirection_parsing_set(args, redirect) == FAIL)
			return (FAIL);
	}
	else
	{
		i = heredoc_herestring_parsing(args, redirect, start);
		if (i == FAIL)
			return (FAIL);
		else if (i == 2)
			return (2);
	}
	return (SUCCESS);
}
