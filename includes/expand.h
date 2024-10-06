/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 21:13:41 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:13:45 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

char	*find_dollar_signs(char *input, char **envp);
char	*expand_cmd(char *cmd, char **env);
char	**expand_args(char **args, char **env);

#endif
