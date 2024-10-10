/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:12:54 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 20:54:24 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "minishell.h"

int		here_doc(t_ASTNode **node);
int		here_string(t_Redirection **redir);
int		read_line(char **line);

#endif
