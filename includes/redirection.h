/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:12:54 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:50:13 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "minishell.h"

int		here_doc(t_ASTNode **node, char *limiter);
int		here_string(t_Redirection **redir);
int		read_line(char **line);

#endif
