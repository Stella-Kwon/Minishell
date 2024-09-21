/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 18:02:03 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/21 18:04:19 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"


ASTNode *parse_to_Nodes(char **tokens, char **env);


Command *create_command(char ***tokens, char **env);
int is_operator(char **tokens);
int set_redirection(Command **res, char *filename, int direction_type);
int set_heredoc(Command **res, char *limiter);
int set_herestring(Command **res, char *string);
;int set_dallor_vari(Command **res, char *vari);
int redirection_parsing(char ***args, Command **res);
int    heredoc_herestring_dollar_parsing(char ***args, Command **res);
int    parsing_others(char ***args, Command **res);
void free_Command(Command **res);

#endif