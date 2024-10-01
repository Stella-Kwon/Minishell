/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:09:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 05:18:59 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_SIGNAL_H
#define MY_SIGNAL_H

#include "minishell.h"

// volatile sig_atomic_t g_received_signal = 0;
extern int g_received_signal; // 다른 파일에서 사용할 수 있도록 선언

void init_signal();
void init_execution_signal();
#endif