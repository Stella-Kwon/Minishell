/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by skwon2            #+#    #+#             */
/*   Updated: 2024/10/22 22:00:45 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H

#define CHILD 0
#define FORK_FAILURE -1
#define PIPE_FAILURE -1
#include "minishell.h"

int error_exitcode(t_Command **command, char *s, int error_nb);
int cmd_error(t_Command **command, char *s, int error_nb);
int check_path(char *path, t_Command **command);
int check_cmd_script(t_Command **command);
int prepare_cmd(t_Command **command, int last_exit_code);
int find_command_path(t_Command **command);
int check_cmd_error(t_Command **command);
int action_parents(t_Redirection **redir, t_Pipeline **pipeline,
				   t_Command **cmd);
int action_child(t_Command **cmd, t_Redirection **redir);
int action_builtin(t_Command **cmd, t_Redirection **redir);
int redirect_infile_to_stdin(t_Redirection **redir, t_Command **cmd);
int redirect_outfile_to_stdout(t_Redirection **redir, t_Command **cmd);
int heredoc_check(t_ASTNode **node);
int heredoc_preprocess(t_ASTNode **node, char **env, int last_exit_code);
int read_all_heredocs(t_ASTNode **node, char **env, int last_exit_code);
char **read_heredoc_body(char *limiter, char **env, int last_exit_code);
int is_quoted_string(char *str);
int ast_node_execution(t_ASTNode **node);
int andnode_exec(t_ASTNode **node);
int ornode_exec(t_ASTNode **node);
int cmdnode_exec(t_ASTNode **node);
int pipenode_exec(t_ASTNode **node);
int find_and_check_path(t_Command **command, char **path);
int print_error_redir(t_Command **cmd, char *filename, int redir_errno);
void set_last_exitcode_and_root(t_ASTNode **node, int last_exit_code,
								t_ASTNode **root);
void get_last_exitcode(t_ASTNode **node, int *last_exitcode);
int check_heredoc(t_ASTNode **node, int exitcode);
int node_command_without_cmd(t_ASTNode **node);
#endif
