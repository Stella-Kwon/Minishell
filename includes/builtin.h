/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:11 by hlee-sun          #+#    #+#             */
/*   Updated: 2024/10/18 23:22:41 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

int		append_to_env(char *str, char ***envp_ptr);
int		check_and_set_path(t_Command *command);
int		cd(t_Command *command);
int		echo(t_Command *command);
int		env(t_Command *command);
int		unset(t_Command *command);
int		mini_exit(t_Command *command);
int		export(t_Command *command);
int		pwd(t_Command *command);
int		copy_envp(char **dest, char **src, size_t len);
int		print_error_cd(t_Command *command, char *s, int error_nb);
int		print_sorted_envp(char **envp);
int		builtin_filesystem(t_Command *command);
int		builtin_with_output(t_Command *command);
int		add_env_item(t_Command *command, char *var_name, char *value);
char	*get_current_dir(char *prefix);
char	*get_env_value(const char *key, char **envp);
void	print_strs(char **str_arr);
void	delete_str_array(char ***arr);
void	sort_envp(size_t len, char **envp);
void	currdir_to_old_pwd(t_Command *command);
size_t	get_str_len(char **str_arr);

#endif
