/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/18 23:09:03 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

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
int		is_special_character(char c);
int		check_input(const char *input);
int		store_str(t_For_tokenize *tokenize);
char	*store_words(char **start);
char	*check_set(t_For_tokenize *tokenize, char ref);
char	**tokenize_input(char **input, int *last_exit_code, \
char ***local_env);
void	update_quotes_and_depth(int *single_quote, int *double_quote, \
								int *depth, char c);
int		read_line(char **line);
int		handle_whitespace(t_For_tokenize *tokenize);
#endif
