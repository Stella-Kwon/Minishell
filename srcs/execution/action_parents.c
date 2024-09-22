/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_parents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:28:46 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/22 20:28:58 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int action_parents(Command *cmd, Pipeline *pipeline)
{
    waitpid(pipeline->pid, &cmd->wstatus, 0);
    cmd->exitcode = waitpid_status(cmd->wstatus);
    return (cmd->exitcode);
}