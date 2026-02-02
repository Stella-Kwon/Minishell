/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 21:00:00 by skwon2            #+#    #+#             */
/*   Updated: 2026/01/29 21:00:00 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void child_spawned(void)
{
    g_child_count++;
    g_no_child = (g_child_count == 0);
}

void child_reaped(void)
{
    if (g_child_count > 0)
        g_child_count--;
    g_no_child = (g_child_count == 0);
}
