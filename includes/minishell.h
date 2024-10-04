/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:11:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 05:19:06 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BUFFSIZE
#  define BUFFSIZE 50
# endif

# define TRUE 1
# define FALSE 0

# define SUCCESS 0
# define FAIL 1

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/param.h>
# include <termios.h>
# include <readline/readline.h>
# include <stddef.h>
# include <readline/history.h>
# include "my_signal.h"
# include "utils.h"
# include "errors.h"
# include "structs.h"
# include "tokenize.h"
# include "redirection.h"
# include "parsing.h"
# include "builtin.h"
# include "expand.h"
# include "execution.h"

#endif
