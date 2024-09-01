/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:31:24 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 18:00:14 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int set_ehcoctl(int flag);
char *ft_strtok(char **input, const char *delimiters);
char **ft_realloc(char **token, int position, int *bufsize);

#endif