/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:06:08 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:57:54 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"

# define LOG_FILE "minishell.log"

int		log_errors(char *tokens, char *msg);
int		handle_258_exitcode_print(char *msg);
void	handle_error(t_Command **command, char *path);

#endif
