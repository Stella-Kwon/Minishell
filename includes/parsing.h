/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:02:03 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/06 21:16:36 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define INVALID -100

t_ASTNode		*parse_to_nodes(char **tokens, char **env);
t_ASTNode		*create_astnode(char ***tokens, t_ASTNode *left, \
t_ASTNode *right, char **env);
t_Command		*create_command(char ***tokens, char **env);
t_Pipeline		*create_pipeline(void);
t_Redirection	*create_redirection(void);
int				is_operator(char **tokens);
int				operation_parsing(char ***tokens, t_ASTNode **left_node, \
char **env);
int				set_redirection(t_Redirection **redirect, char *filename, \
int direction_type);
int				set_heredoc(t_Redirection **redirect, char *limiter);
int				set_herestring(t_Redirection **redirect, char *string);
int				redirection_parsing(char ***args, t_Redirection **redirect, \
int start);
int				heredoc_herestring_parsing(char ***args, \
t_Redirection **redirect, int start);
int				parsing_others(char ***args, \
t_Redirection **redirect, int start);
void			free_command(t_Command **res);
void			free_astnode(t_ASTNode **node);
void			print_astnode(t_ASTNode *node, int depth);
int				initialize_astnode(t_ASTNode **node, char ***tokens);
int				get_direction_type(char *token);
void			remove_args_after_redirection(char ***args);
int				is_redirection(char **token);
#endif
