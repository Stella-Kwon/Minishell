/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:51:43 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 21:40:43 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_ehcoctl(int flag)
{
	struct termios	attr;

	if (tcgetattr(STDIN_FILENO, &attr) == -1)
		return (FAIL);
	if (!flag)
		attr.c_lflag |= ECHOCTL;
	else
		attr.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &attr) == -1)
		return (FAIL);
	return (SUCCESS);
}