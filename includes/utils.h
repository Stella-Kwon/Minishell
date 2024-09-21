/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:31:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 19:04:51 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int set_ehcoctl(int flag);
char **ft_realloc(char **token, int position, int *bufsize);
char *ft_strndup(char *str, int n);
int ft_isspace(char c);
int ft_strcmp(const char *line, const char *limiter);
int waitpid_status(int wstatus);
#endif