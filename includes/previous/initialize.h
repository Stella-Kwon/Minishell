/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:44:36 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/04 17:40:54 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITIALIZE_H
# define INITIALIZE_H

# include "minishell.h"

Command *create_command();
ASTNode *create_ASTnode();
ASTlist *create_ast_list();
void add_ast_node(ASTlist *ast_list, ASTNode *new_node);
#endif