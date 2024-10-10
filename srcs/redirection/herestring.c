/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:20:17 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/10 21:46:54 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	here_string(t_Redirection **redir)
{
	(*redir)->infile = open(".herestring.tmp", \
	O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*redir)->infile == -1)
		return (log_errors("Failed to open file in here_string", \
		".herestring.tmp"));
	if (write((*redir)->infile, (*redir)->herestring_str, \
	ft_strlen((*redir)->herestring_str)) == -1)
		return (log_errors("Failed to write in here_string", ""));
	close((*redir)->infile);
	(*redir)->infile = open(".herestring.tmp", O_RDONLY);
	if ((*redir)->infile == -1)
		return (log_errors("Failed to open file \
		in here_string for reading", ".herestring.tmp"));
	if (unlink(".herestring.tmp") == -1)
		return (log_errors("Failed to unlink in here_string", ""));
	return (SUCCESS);
}
