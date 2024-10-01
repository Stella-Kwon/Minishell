/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 13:06:08 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 22:52:10 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"
# include <time.h> //이거 추후에 뺴

// 로그 파일 경로를 정의
# define LOG_FILE "minishell.log"

int log_errors(char *tokens, char *msg);
int handle_error(int custom_error_code);

int handle_258_exitcode_print(char *msg);
#endif