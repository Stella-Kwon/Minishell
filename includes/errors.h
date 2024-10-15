/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:06:08 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:52:10 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"

# define LOG_FILE "minishell.log"

int	log_errors(char *tokens, char *msg);
void handle_error(t_Command **command, char *path);
int handle_258_exitcode_print(char *msg);

#endif