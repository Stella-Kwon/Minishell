/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:11:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 21:56:03 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/param.h>
# include <sys/wait.h>
# include <termios.h>
# include <readline/readline.h>
# include <stddef.h>
# include <readline/history.h>
# include <signal.h>
# include "my_signal.h"
# include "structs.h"
# include "utils.h"
# include "errors.h"
# include "tokenize.h"
# include "redirection.h"
# include "parsing.h"
# include "builtin.h"
# include "expand.h"
# include "execution.h"

# ifndef BUFFSIZE
#  define BUFFSIZE 50
# endif

// for echoctl
#define	SET	1
#define UNSET 0

# define TRUE 1
# define FALSE 0

# define SUCCESS 0
# define FAIL 1

extern int	g_received_signal;

int		local_env_copy(char **env, char ***local_env);
char	**process_input_to_tokens(char *input, int *last_exit_code, \
								char ***local_env);
char	*get_user_input(int *last_exit_code, char ***local_env);

#endif