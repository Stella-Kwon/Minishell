/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:01:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/29 22:16:43 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H

# include "minishell.h"

char	*check_set(char **input, char **start, char **tokens, int *token_count, char ref);
void	update_quotes_and_depth(int *single_quote, int *double_quote, \
		int *depth, char c);
int		readline_again(char **input, t_Set *set, char **start);
int		handle_pipe_and_or(char **input, char **tokens, char **start, \
		int *token_count);
int		handle_And_and_background(char **input, char **tokens, char **start, \
		int *token_count);
int		handle_input_redirection(char **tokens, char **start, int *token_count);
int		handle_output_redirection(char **tokens, char **start, \
		int *token_count);
int		handle_token(char **tokens, char **start, int *token_count, int len);
int		handle_set(char **tokens, char **start, int *token_count, \
		char **input, char ref);
char	*store_words(char **start);
int		check_operation_next(char **input, char **start, int flag);
int		check_first_input(char **start);
char	**tokenize_input(char **input);

#endif

