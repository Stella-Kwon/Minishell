/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/23 22:40:08 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

struct	s_astnode;

typedef struct s_Command
{
	char				*cmd;
	char				***env;
	char				**args;
	char				**tmp_args;
	int					exitcode;
	int					wstatus;
	struct s_astnode	**root_node;
}	t_Command;

typedef struct s_Set
{
	int		depth;
	int		single_quote;
	int		double_quote;
	char	*tmp_start;
	char	*tmp_end;
}	t_Set;

typedef struct s_Line
{
	char	c;
	int		r_byte;
	int		i;
	int		buf_size;
}	t_Line;

typedef struct s_Pipeline
{
	int		fd[2];
	pid_t	pid;
	pid_t	left_pid;
	pid_t	right_pid;
}	t_Pipeline;

typedef enum e_redirect
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND
}	t_REDIRECT;

typedef enum e_nodetype
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR
}	t_NodeType;

typedef struct s_remove_args
{
	char	**new_args;
	int		i;
	int		origin_i;
	int		buffersize;
}	t_rm_args;

typedef struct s_Redirection
{
	int		infile;
	int		outfile;
	int		heredoc_infile;
	char	*in_filename;
	char	*out_filename;
	int		direction_type;
	char	**heredoc_limiter;
	int		heredoc_i;
	char	*herestring_str;
	int		heredoc_buffsize;
	int		errno_in;
	int		errno_out;
}	t_Redirection;

typedef struct s_astnode
{
	t_NodeType			type;
	t_Command			*command;
	t_Redirection		*redir;
	t_Pipeline			*pipeline;
	struct s_astnode	*left;
	struct s_astnode	*right;
	int					last_exitcode;
}	t_ASTNode;

typedef struct s_Dollar
{
	size_t	len;
	size_t	tmp_len;
	char	*tmp;
	size_t	i;
	size_t	tmp_i;
	size_t	var_len;
	size_t	var_start;
	char	*var;
	char	*var_value;
}	t_Dollar;

typedef struct s_For_tokenize
{
	char	*input;
	char	*start;
	char	**tokens;
	int		token_count;
	int		buffsize;
}	t_For_tokenize;

typedef struct s_line
{
	char	c;
	int		r_byte;
	int		i;
	int		buf_size;
}	t_line;

#endif
