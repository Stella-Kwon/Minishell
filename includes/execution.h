/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/09 23:26:10 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# define CHILD 0
# define FORK_FAILURE -1
# define PIPE_FAILURE -1
# include "minishell.h"

int		error_exitcode(t_Command **command, char *s, int error_nb);
int		cmd_error(t_Command **command, char *s, int error_nb);
int		check_path(char *path, t_Command **command);
int		check_cmd_script(t_Command **command);
int 	prepare_cmd(t_Command **command, int last_exit_code);
int		execute_cmd(t_Command **command);
int		check_cmd_error(t_Command **command);
int		action_parents(t_Redirection **redir, t_Pipeline **pipeline);
int		action_child(t_Command **cmd, t_Redirection **redir);
int		common_pre_child(t_Redirection **redir);
int		heredoc_check(t_ASTNode **node);
int		ast_node_execution(t_ASTNode **node);
int		andnode_exec(t_ASTNode **node);
int		ornode_exec(t_ASTNode **node);
int		cmdnode_exec(t_ASTNode **node);
int		pipenode_exec(t_ASTNode **node);
void	set_last_exitcode(t_ASTNode	**node, int last_exit_code);
void	get_last_exitcode(t_ASTNode	**node, int *last_exitcode);
char	*ft_strcpy(char *dst, const char *src);
int		dup_and_close(int oldfd, int newfd);

#endif

