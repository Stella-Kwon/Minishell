/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:11 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/12 22:19:17 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

char	**create_env(char **curr_envp);
void	delete_str_array(char ***str_arr);
int		append_to_env(char *str, char ***envp_ptr);
char	*get_env_value(const char *key, char **envp);
void	check_and_set_path(t_Command *command);
int		cd(t_Command *command);
int		echo(t_Command *command);
int		env(t_Command *command);
int		unset(t_Command *command);
int		mini_exit(t_Command *command);
int		export(t_Command *command);
int		pwd(t_Command *command);
void	export_check(t_Command *command);
size_t	get_str_len(char **str_arr);
int		copy_envp(char **dest, char **src, size_t len);
void	print_strs(char **str_arr);
void	sort_envp(size_t len, char **envp);
// void	ft_perror(char *message);
int		print_error_cd(t_Command *command, char *s, int error_nb);
int		builtin_filesystem(t_Command *command);
int 	exec_command_with_env(t_Command *command, char **env, int last_exitcode);
int		builtin_with_output(t_Command *command);

#endif
