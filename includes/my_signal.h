/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:09:42 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/18 23:22:52 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_SIGNAL_H
# define MY_SIGNAL_H

# include "minishell.h"

void	init_signal(void);
void	init_execution_signal(void);
void	readline_signal(void);
#endif
