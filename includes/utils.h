/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:31:24 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/13 17:01:50 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int		set_ehcoctl(int flag);
char    *ft_realloc_single(char *token, size_t position, size_t *bufsize);
char	**ft_realloc_double(char **token, size_t position, size_t *bufsize);
char	*ft_strndup(char *str, int n);
int		ft_isspace(char c);
int		ft_strcmp(const char *line, const char *limiter);
int		waitpid_status(int wstatus);
void	free_one(void **ptr);
void	all_free(char ***res);

#endif