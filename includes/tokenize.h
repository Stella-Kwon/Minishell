/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/20 19:07:53 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

char *check_set(char *input, char **start, char **tokens, int token_count, char c);

char *readline_again(char *input, int quote, char c);


int handle_pipe_and_or(char **tokens, char **start, int *token_count);
int handle_And_and_background(char **tokens, char **start, int *token_count);
int handle_input_redirection(char **tokens, char **start, int *token_count);
int handle_output_redirection(char **tokens, char **start, int *token_count);
int handle_token(char **tokens, char **start, int *token_count, int len);
int handle_set(char **tokens, char **start, int *token_count, char c);

char *store_words(char **start);

//이걸위한
char **tokenize_input(char *input);

#endif