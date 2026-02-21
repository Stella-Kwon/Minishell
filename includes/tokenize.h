/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/21 15:50:35 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
#define TOKENIZE_H

#define END_OF_INPUT 4
#include "minishell.h"

int readline_again_for_quotes(t_For_tokenize *tokenize, t_Set *set, char ref);
int readline_again_after_operator(t_For_tokenize *tokenize);
int readline_again_for_heredoc(t_For_tokenize *tokenize);
int handle_pipe_and_or(t_For_tokenize *tokenize);
int handle_and(t_For_tokenize *tokenize);
int handle_input_redirection(t_For_tokenize *tokenize);
int handle_output_redirection(t_For_tokenize *tokenize);
int handle_token(t_For_tokenize *tokenize, int len);
int handle_set(t_For_tokenize *tokenize, char ref);
int check_operation_next(t_For_tokenize *tokenize);
int redirect_operation_error(char *start);
int check_quotes(const char *input);
int store_str(t_For_tokenize *tokenize);
char *store_words(t_For_tokenize *tokenize);
int check_set(t_For_tokenize *tokenize, char ref);
char **tokenize_input(char **tmp_input, t_TokenizeResult *result, char ***env, int *last_exit_code);
char *rm_quotes(char *str);
int handle_special_tokens(t_For_tokenize *tokenize);
void set_ref_and_tmp_start(char *start, t_Set *set, char *ref);
int read_prompt_line(char **line);
void initialize_set(char *start, t_Set *set);
void check_pairs(int *single_quote, int *double_quote, int *parenthesis, char c);
int check_input_set(const char *input, char ref);

#endif
