/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:11:15 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 21:56:07 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <termios.h>
#include <readline/readline.h>
#include <stddef.h>
#include <readline/history.h>
#include <sys/stat.h>

#include "structs.h"
#include "signal.h"
#include "heredoc.h"
#include "execution.h"
#include "parsing.h"
#include "tokenize.h"
#include "utils.h"
#include "errors.h"
#include "builtin.h"
#include "redirection.h"
#include "expand.h"

#define LOG_FILE "minishell.log"

#define HEREDOC_BUFFSIZE 10

#ifndef TOKEN_BUFFSIZE
#define TOKEN_BUFFSIZE 50
#endif

#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define FAIL 1

int local_env_copy(char **env, char ***local_env);
t_TokenizeResult process_input_to_tokens(const char *input, char ***env,
										 int *last_exit_code);
const char *get_user_input(void);

#endif