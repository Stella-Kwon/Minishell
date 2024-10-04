/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:20:17 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:59:31 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:20:17 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:59:31 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	here_string(t_Redirection **redir)
{
	(*redir)->infile = open(".herestring.tmp", \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*redir)->infile == -1)
		return (log_errors(".herestring.tmp", \
			"Failed to open file in here_string"));
	if (write((*redir)->infile, (*redir)->herestring_str, \
			ft_strlen((*redir)->herestring_str)) == -1)
		return (log_errors("Failed to write in here_string", ""));
	close((*redir)->infile);
	(*redir)->infile = open(".heredoc.tmp", O_RDONLY);
	if ((*redir)->infile == -1)
		return (log_errors(".herestring.tmp", \
			"Failed to open file in here_string"));
	if (unlink(".herestring.tmp") == -1)
		return (log_errors("Failed to unlink in here_string", ""));
	return (SUCCESS);
}

