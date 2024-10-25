/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 03:35:30 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/25 21:30:10 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_newline_and_free(char **input)
{
	char	*tmp;

	tmp = ft_strjoin(*input, "\n");
	if (!tmp)
		return (log_errors("Failed to ft_strjoin in heredoc_child", ""));
	free_one((void **)input);
	*input = tmp;
	return (SUCCESS);
}

static void	check_empty_input_and_dollar_sign(char **new_input, \
											t_ASTNode **node, char **copy_new)
{
	(void)copy_new;
	if ((*node)->command)
	{
		if (find_dollar_signs(new_input, *((*node)->command->env), \
							(*node)->last_exitcode) == FAIL)
			log_errors("Command expansion failed", *new_input);
	}
}

static void	free3_exit(char **s1, char **s2, char **s3, t_Command *cmd)
{
	free_one((void **)s1);
	free_one((void **)s2);
	free_one((void **)s3);
	free_exit(&cmd, SUCCESS);
}

void	write_file(char *limiter, char **new_input, \
t_ASTNode **node, char	*copy_new)
{
	if (write((*node)->redir->heredoc_infile, *new_input, \
	ft_strlen(*new_input)) == -1)
	{
		log_errors("Failed to write in heredoc child", "");
		free3_exit(&copy_new, new_input, &limiter, (*node)->command);
	}
}

void	handle_input(char *limiter, char **new_input, \
t_ASTNode **node, int check)
{
	char	*copy_new;

	while (1)
	{
		*new_input = readline("> ");
		if (!(*new_input))
			exit(3);
		copy_new = ft_strdup(*new_input);
		if (!copy_new)
			log_errors ("Failed to copy_new ft_strdup", copy_new);
		if (check == FALSE)
			check_empty_input_and_dollar_sign(new_input, node, &copy_new);
		if (ft_strcmp(copy_new, limiter) == 0)
		{
			free_one((void **)&copy_new);
			break ;
		}
		if (append_newline_and_free(new_input) == FAIL)
			free3_exit(&copy_new, new_input, &limiter, (*node)->command);
		write_file(limiter, new_input, node, copy_new);
		free_one((void **)new_input);
		free_one((void **)&copy_new);
	}
}
