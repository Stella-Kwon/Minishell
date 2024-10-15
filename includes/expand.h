/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:41 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/15 04:34:28 by hlee-sun         ###   ########.fr       */
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
char	*find_dollar_signs(char *input, char **env, int last_exitcode);
char	*expand_cmd(char *cmd, char **env, int last_exitcode);
char	**expand_args(char **args, char **env, int last_exitcode);

#endif