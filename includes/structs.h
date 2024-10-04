/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 20:03:49 by suminkwon        ###   ########.fr       */
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
	char	**env;
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
	int		tmp_fd;
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

typedef struct s_Redirection
{
	int		infile;
	int		outfile;
	char	*filename;
	int		direction_type;
	char	*heredoc_limiter;
	char	*herestring_str;
	char	*dollar_vari;
}	t_Redirection;

typedef struct s_ASTNode
{
	t_NodeType			type;
	t_Command			*command;
	t_Redirection		*redir;
	t_Pipeline			*pipeline;
	struct s_ASTNode	*left;
	struct s_ASTNode	*right;
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

#endif
