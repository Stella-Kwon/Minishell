/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:12:54 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 11:50:13 by sukwon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "minishell.h"

int		here_doc(t_ASTNode **node, char *limiter);
int		here_string(t_Redirection **redir);
int		read_line(char **line);

#endif
