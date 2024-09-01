/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:11:15 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/29 14:45:58 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
#define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <termios.h> //입출력 및 터미널 속성에 관련한 헤더
# include <readline/readline.h>
# include <readline/history.h>
# include "signal.h"
# include "utils.h"
# include "errors.h"
# include "structs.h"
# include "initialize.h"

// typedef int boolean; //을 하던
# define TRUE 1
# define FALSE 0

# define SUCCESS 0
# define FAIL 1


#endif
