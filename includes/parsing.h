/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:02:03 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/02 00:45:53 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

# define INVALID -100

t_ASTNode	*parse_to_Nodes(char **tokens, char **env);
t_Command	*create_command(char ***tokens, char **env);
int			is_operator(char **tokens);
int			set_redirection(t_Redirection **redirect, char *filename, \
			int direction_type);
int			set_heredoc(t_Redirection **redirect, char *limiter);
int			set_herestring(t_Redirection **redirect, char *string);
int			set_dollar_vari(t_Redirection **redirect, char *vari);
int			redirection_parsing(char ***args, t_Redirection **redirect);
int			heredoc_herestring_dollar_parsing(char ***args, \
			t_Redirection **redirect);
int			parsing_others(char ***args, t_Redirection **redirect, int start);
void		free_Command(t_Command **res);
void		print_ASTNode(t_ASTNode *node, int depth);

#endif
