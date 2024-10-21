/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:09:42 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/20 16:50:09 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_SIGNAL_H
# define MY_SIGNAL_H

# include "minishell.h"

void    signal_set(void (*sigint)(int), void (*sigquit)(int)) ;
void    signal_no_input(int signal);
void    execution_sigint(int signal);
void    execution_sigquit(int signal);
#endif
