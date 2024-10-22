/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:06:08 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 20:40:09 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"

# define LOG_FILE "minishell.log"

int	log_errors(char *tokens, char *msg);
int	handle_258_exitcode_print(char *msg);
int	handle_error(char *path);
int check_specific_error(char *path, int ernbr);

#endif
