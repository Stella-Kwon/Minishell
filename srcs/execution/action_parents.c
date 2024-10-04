/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:05 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:19:22 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int action_parents(t_Command *cmd, t_Pipeline *pipeline)
{
    waitpid(pipeline->pid, &cmd->wstatus, 0);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}