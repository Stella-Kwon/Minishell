/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:31:24 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/10 22:41:33 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int		set_ehcoctl(int flag);
char    *ft_realloc_single(char *token, int position, int *bufsize);
char	**ft_realloc_double(char **token, int position, int *bufsize);
char	*ft_strndup(char *str, int n);
int		ft_isspace(char c);
int		ft_strcmp(const char *line, const char *limiter);
int		waitpid_status(int wstatus);
void	free_one(void **ptr);
void	all_free(char ***res);

#endif