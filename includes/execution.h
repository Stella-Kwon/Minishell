/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 21:28:03 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# define CHILD 0
# define FORK_FAILURE -1
# define PIPE_FAILURE -1
# include "minishell.h"

char	*find_dollar_signs(char *input, char **envp);
int		error_exitcode(t_Command **command, char *s, int error_nb);
int		cmd_error(t_Command **command, char *s, int error_nb);
int		check_path(char *path, t_Command **command);
int		check_cmd_script(t_Command **command);
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
int		pipe_action_parents(t_Command *cmd, t_Pipeline *pipeline);
int		pipe_execute_command(t_ASTNode **node);
int		pipenode_exec(t_ASTNode **node);
int		execute_node(t_ASTNode **node);
int		check_null_cmd(t_Command *command);
char	*ft_strcpy(char *dst, const char *src);
#endif
