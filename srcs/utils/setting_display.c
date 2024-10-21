/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:51:43 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/13 21:40:43 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void set_echoctl(int set_echoctl)
{
	struct termios info;

	ft_memset(&info, 0, sizeof(struct termios));
	if (set_echoctl == SET_ECHOCTL)
	{
		tcgetattr(0, &info);
		info.c_lflag |= ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
	else
	{
		tcgetattr(0, &info);
		info.c_lflag &= ~ECHOCTL;
		tcsetattr(0, TCSANOW, &info);
	}
}