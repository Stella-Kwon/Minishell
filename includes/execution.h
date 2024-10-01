/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 20:58:46 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

#include "minishell.h"

int action_child(Command *cmd, Pipeline *pipeline, Redirection *redir);
int action_parents(Command *cmd, Pipeline *pipeline);
int heredoc_check(ASTNode **node);

int ast_node_execution(ASTNode **node);
int CMDnode_exec(ASTNode **node);
int ORnode_exec(ASTNode **node);
int ANDnode_exec(ASTNode **node);

int PIPEnode_exec(ASTNode **node);
#endif