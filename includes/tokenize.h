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
# define TOKENIZE_H

# define END_OF_INPUT 4
# include "minishell.h"

int		readline_again(t_For_tokenize *tokenize, t_Set *set);
int		handle_pipe_and_or(t_For_tokenize *tokenize);
int		handle_and_and_background(t_For_tokenize *tokenize);
int		handle_input_redirection(t_For_tokenize *tokenize);
int		handle_output_redirection(t_For_tokenize *tokenize);
int		handle_token(t_For_tokenize *tokenize, int len);
int		handle_set(t_For_tokenize *tokenize, char ref);
int		check_operation_next(t_For_tokenize *tokenize);
int		check_first_input(t_For_tokenize *tokenize);
int		redirect_operation_error(char *start);
int		check_input(const char *input);
int		store_str(t_For_tokenize *tokenize);
char	*store_words(t_For_tokenize *tokenize);
char	*check_set(t_For_tokenize *tokenize, char ref);
char	**tokenize_input(char **input, int *last_exit_code, \
char ***local_env, char **tmp_input);
int		read_line(char **line);
int		handle_whitespace(t_For_tokenize *tokenize);
void	initialize_set(char *start, t_Set *set);
void	update_quotes_and_depth(int *single_quote, int *double_quote, \
int *depth, char c);
int		check_quotes_and_depth(t_For_tokenize *tokenize, \
t_Set *set, char ref);
void	check_quotes_in_loop(t_Set *set, char ref, int *count);
char	*rm_quotes(char *str);
int		handle_special_tokens(t_For_tokenize *tokenize);
int		rm_quote_buffer(t_For_tokenize *tokenize, char **buffer, char **cp_input);
int		join_inputs(char **tokenize_input, char **new_input);
void	set_ref_and_tmp_start(char *start, t_Set *set, char *ref);
#endif