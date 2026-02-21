/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 22:00:45 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
#define HEREDOC_H

#include "structs.h"

int heredoc_check(t_ASTNode **node);
int prepare_heredoc_herestring(t_ASTNode **node, char **env, int last_exit_code);
int process_herestring(t_ASTNode **node, char **env, int last_exit_code);

#endif