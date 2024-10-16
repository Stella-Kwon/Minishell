/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:41 by hlee-sun         #+#    #+#              */
/*   Updated: 2024/10/15 22:55:59 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

int		expand_error(char *command);
int		expand_value(t_Dollar *dol);

int		handle_empty_cmd(t_Command **command);
int		for_exit_code(t_Dollar *dol, int last_exitcode);
int		handle_single_quote(char *input, t_Dollar *dol);
int		handle_double_quote(char *input, t_Dollar *dol, char **env, \
						int last_exitcode);
int		for_curly_braces(char *input, t_Dollar *dol, char **env, \
					int last_exitcode);
int		for_dollar_sign(char *input, t_Dollar *dol, char **env, \
						int last_exitcode);
void	merge_quoted_args(char **args, int *argc);
char	*remove_quotes(char *s);
int		find_dollar_signs(char **in_out, char **env, int last_exitcode);
void	expand_cmd_args(t_Command *command, int last_exitcode);

#endif
