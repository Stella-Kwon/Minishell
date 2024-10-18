/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sukwon <sukwon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:02:03 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/16 11:53:23 by sukwon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define INVALID -100

t_ASTNode		*parse_to_nodes(char **tokens, char ***env);
t_ASTNode		*create_astnode(char ***tokens, t_ASTNode *left, \
								t_ASTNode *right, char ***env);
t_Command		*create_command(char ***tokens, char ***env);
t_Pipeline		*create_pipeline(void);
t_Redirection	*create_redirection(void);
int				is_operator(char **tokens);
int				operation_parsing(char ***tokens, t_ASTNode **left_node, \
									char ***env);
int				set_redirection(t_Redirection **redirect, char ***args, \
								int direction_type);
int				set_heredoc(t_Redirection **redirect, char *limiter);
int				set_herestring(t_Redirection **redirect, char *string);
int				redirection_parsing(char ***args, t_Redirection **redirect, \
									int start);
int				heredoc_herestring_parsing(char ***args, \
										t_Redirection **redirect, int start);
int				parsing_others(char ***args, \
								t_Redirection **redirect, int start);
int				initialize_astnode(t_ASTNode **node, char ***tokens);
int				get_direction_type(char *token);
int				remove_args_after_redirection(char ***args);
int				is_redirection(char *token);
int				put_last_open_infile(t_Redirection **redirect, char ***args, \
									char **filename);
int				rm_quote_filename(t_Redirection **redirect, char ***args, \
									char **filename);
int				restore_new_args(char ***args, t_rm_args *rm);
void			free_command(t_Command **res);
void			free_astnode(t_ASTNode **node);
void			print_astnode(t_ASTNode *node, int depth);
void			remove_arg(char ***args, int index);
#endif
