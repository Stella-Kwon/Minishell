/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:02:03 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/03 18:55:29 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

#define INVALID -100

ASTNode *parse_to_Nodes(char **tokens, char **env);


Command *create_command(char ***tokens, char **env);
int is_operator(char **tokens);
int set_redirection(Redirection **redirect, char *filename, int direction_type);
int set_heredoc(Redirection **redirect, char *limiter);
int set_herestring(Redirection **redirect, char *string);
int set_dollar_vari(Redirection **redirect, char *vari);
int redirection_parsing(char ***args, Redirection **redirect);
int heredoc_herestring_dollar_parsing(char ***args, Redirection **redirect);
int parsing_others(char ***args, Redirection **redirect, int start);
void free_Command(Command **res);
void free_ASTNODE(ASTNode **node);
    // print
void print_ASTNode(ASTNode *node, int depth);
#endif