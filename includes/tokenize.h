/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/05 03:12:39 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

char	*check_set(t_For_tokenize *tokenize, char ref);
void	update_quotes_and_depth(int *single_quote, int *double_quote, \
		int *depth, char c);
int		readline_again(t_For_tokenize *tokenize, t_Set *set);
int		handle_pipe_and_or(t_For_tokenize *tokenize);
int		handle_and_and_background(t_For_tokenize *tokenize);
int		handle_input_redirection(t_For_tokenize *tokenize);
int		handle_output_redirection(t_For_tokenize *tokenize);
int		handle_token(t_For_tokenize *tokenize, int len);
int		handle_set(t_For_tokenize *tokenize, char ref);
char	*store_words(char **start);
int		check_operation_next(t_For_tokenize *tokenize, int flag);
int		check_first_input(t_For_tokenize *tokenize);
char	**tokenize_input(char **input);
int		redirect_operation_error(char *start);

#endif

