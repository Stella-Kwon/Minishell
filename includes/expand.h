/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:41 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 15:06:53 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

char	*find_dollar_signs(char *input, char **env, int last_exitcode);
char	*expand_cmd(char *cmd, char **env, int last_exitcode);
char	**expand_args(char **args, char **env, int last_exitcode);
int		expand_error(char *command);
char	*remove_quotes(char *s);
int		handle_empty_cmd(t_Command **command);

int	for_dollar_sign(char *input, t_Dollar *dol, char **env, int last_exitcode);
#endif
