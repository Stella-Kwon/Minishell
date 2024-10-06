/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:13:14 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

char	**create_env(char **curr_envp);
void	delete_str_array(char ***str_arr);
int		append_to_env(char *str, char ***envp_ptr);
char	*get_env_value(const char *key, char **envp);
void	check_and_set_path(char **args, char ***envp_ptr);
int		cd(char **args, char ***envp_ptr);
int		echo(char **args, char ***envp);
int		env(char **args, char ***envp);
int		unset(char **args, char ***envp_ptr);
int		mini_exit(char **args, char ***envp_ptr);
int		export(char **args, char ***envp_ptr);
int		pwd(char **args, char ***envp);
void	export_check(char **args, char ***envp_ptr);
size_t	get_str_len(char **str_arr);
int		copy_envp(char **dest, char **src, size_t len);
void	print_strs(char **str_arr);
void	sort_envp(size_t len, char **envp);
void	ft_perror(char *message);
void	print_error_cd(char *filename);
int		builtin(t_Command *command);

#endif
