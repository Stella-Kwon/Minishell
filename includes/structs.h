
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/09 02:16:12 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_Command
{
	char	*cmd;
	char	***env;
	char	**args;
	int		exitcode;
	int		wstatus;
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
	int		tmp_infile;
	char	*filename;
	int		direction_type;
	char	*heredoc_limiter;
	char	*herestring_str;
}	t_Redirection;

typedef struct s_ASTNode
{
	t_NodeType			type;
	t_Command			*command;
	t_Redirection		*redir;
	t_Pipeline			*pipeline;
	struct s_ASTNode	*left;
	struct s_ASTNode	*right;
	int					last_exitcode;
}	t_ASTNode;

typedef struct s_Dollar
{
	size_t	len;
	size_t	out_len;
	char	*output;
	size_t	i;
	size_t	out_i;
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
}	t_For_tokenize;

#endif
