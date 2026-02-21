/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:20:17 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/13 22:45:43 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int here_string(t_Redirection **redir)
{
	(*redir)->infile = open(".herestring.tmp",
							O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*redir)->infile == -1)
		return (log_errors("Failed to open file in here_string",
						   ".herestring.tmp"));
	if (write((*redir)->infile, (*redir)->herestring_str,
			  ft_strlen((*redir)->herestring_str)) == -1)
		return (log_errors("Failed to write in here_string", ""));
	close((*redir)->infile);
	(*redir)->infile = open(".herestring.tmp", O_RDONLY);
	if ((*redir)->infile == -1)
		return (log_errors("Failed to open file in here_string for \
		reading",
						   ".herestring.tmp"));
	if (unlink(".herestring.tmp") == -1)
		return (log_errors("Failed to unlink in here_string", ""));
	return (SUCCESS);
}

int process_herestring(t_ASTNode **node, char **env, int last_exit_code)
{
	char *result;
	char *tmp;
	int should_expand;

	if (!(*node)->redir->herestring_str)
		return (SUCCESS);
	if ((*node)->redir->last_stdin_type != 3)
		return (SUCCESS);
	should_expand = ((*node)->redir->herestring_str[0] == '"');
	if (should_expand && find_dollar_signs(&(*node)->redir->herestring_str, env, last_exit_code) == FAIL)
		return (FAIL);
	tmp = rm_quotes((*node)->redir->herestring_str);
	if (!tmp)
		return (log_errors("Failed rm_quotes in process_herestring", ""));
	free((*node)->redir->herestring_str);
	(*node)->redir->herestring_str = tmp;
	result = ft_strjoin((*node)->redir->herestring_str, "\n");
	if (!result)
		return (log_errors("Failed malloc in process_herestring", ""));
	free((*node)->redir->herestring_str);
	(*node)->redir->herestring_str = result;
	if (here_string(&(*node)->redir) != SUCCESS)
		return (FAIL);
	return (SUCCESS);
}
