/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by skwon2            #+#    #+#             */
/*   Updated: 2026/03/05 11:32:08 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum e_redirect
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND
} t_REDIRECT;

typedef enum e_nodetype
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
} t_NodeType;

typedef struct s_For_tokenize
{
	char *input;
	char *start;
	char **tokens;
	int token_count;
	int buffsize;

	char **heredoc_limiters;
	char **heredoc_bodies;
	int *heredoc_quoted;
	int heredoc_count;
	int heredoc_buffsize;
	char ***env;
	int *last_exit_code;

} t_For_tokenize;

typedef struct s_TokenizeResult
{
	char **tokens;
	char **heredoc_limiters;
	char **heredoc_bodies;
	int *heredoc_quoted;
	int heredoc_count;
	int current_heredoc_index;
} t_TokenizeResult;

typedef struct s_Redirection
{
	int infile;
	int outfile;
	char *in_filename;
	char *out_filename;
	int direction_type;
	char **heredoc_limiter;
	char **heredoc_body;
	int heredoc_i;
	char *herestring_str;
	int heredoc_buffsize;
	int last_stdin_type;
	int errno_in;
	int errno_out;
} t_Redirection;

typedef struct s_Command
{
	char *cmd;
	char ***env;
	char **args;
	char **tmp_args;
	int exitcode;
	int wstatus;
	struct s_astnode **root_node;
} t_Command;

typedef struct s_Pipeline
{
	int fd[2];
	pid_t pid;
	pid_t left_pid;
	pid_t right_pid;
} t_Pipeline;

typedef struct s_astnode
{
	t_NodeType type;
	t_Command *command;
	t_Redirection *redir;
	t_Pipeline *pipeline;
	struct s_astnode *left;
	struct s_astnode *right;
	int last_exitcode;
} t_ASTNode;

typedef struct s_Set
{
	int single_quote;
	int double_quote;
	int parenthesis;
	char *tmp_start;
	char *tmp_end;
} t_Set;

typedef struct s_remove_args
{
	char **new_args;
	int i;
	int origin_i;
	int buffersize;
} t_rm_args;

typedef struct s_Dollar
{
	size_t len;
	size_t tmp_len;
	char *tmp;
	size_t i;
	size_t tmp_i;
	size_t var_len;
	size_t var_start;
	char *var;
	char *var_value;
} t_Dollar;

#endif
