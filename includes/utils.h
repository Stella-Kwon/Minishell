/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:31:24 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/16 11:46:29 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

int		set_ehcoctl(int flag);
int		ft_isspace(char c);
int		waitpid_status(int wstatus);
int		ft_strcmp(const char *line, const char *limiter);
void	ft_strcat(char *dest, const char *src);
void	free_one(void **ptr);
void	all_free(char ***res);
char	*ft_strndup(char *str, int n);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3);
char	*ft_realloc_single(char *token, int position, int *bufsize);
char	**ft_realloc_double(char **token, int position, int *bufsize);

#endif
