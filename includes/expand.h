/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:41 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/08 21:29:32 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

char	*find_dollar_signs(char *input, char **env, int last_exit_code);
char	*expand_cmd(char *cmd, char **env, int last_exit_code);
char	**expand_args(char **args, char **env, int last_exit_code);
int		expand_error(char *command);

#endif
