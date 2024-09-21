/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize .h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 19:03:37 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

int *parsing_data_into_astnode(ASTNode **node);
char **token_list(char **input);
char *is_it_set_quote(char **input);
void free_tokens(char **token_list, char **input_new, char *input);
void free_tokenlist(char **token_list);

#endif