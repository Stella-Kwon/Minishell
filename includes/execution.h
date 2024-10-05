/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 04:44:20 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/05 19:58:44 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef EXECUTION_H
# define EXECUTION_H

#define CHILD 0
#define FORK_FAILURE -1
#define PIPE_FAILURE -1

#include "minishell.h"

char	*find_dollar_signs(char *input, char **envp);
int	error_exitcode(Command **command, char *s, int error_nb);
int	cmd_error(Command **command, char *s, int error_nb);
int	check_path(char *path, Command **command);
int	check_cmd_script(Command **command);
int	execute_cmd(Command **command);
int	check_cmd_error(Command **command);
char	*ft_strrchr(const char *s, int c);

int action_parents(Redirection **redir, Pipeline **pipeline);
int action_child(Command **cmd, Redirection **redir, Pipeline **pipeline);
int heredoc_check(ASTNode **node);
int	ast_node_execution(ASTNode **node);
int ANDnode_exec(ASTNode **node);
int ORnode_exec(ASTNode **node);
int CMDnode_exec(ASTNode **node);
int pipe_action_parents(Command **cmd, Redirection **redir, Pipeline **pipeline);
int pipe_execute_command(ASTNode **node);
int PIPEnode_exec(ASTNode **node);

int execute_node(ASTNode **node);

int		remove_quotes(Command **command);
int		check_null_cmd(Command **command);
void	free_arrays(char **str);
char	*ft_strcpy(char *dst, const char *src);

char *expand_cmd(char *cmd, char **env);
char **expand_args(char **args, char **env);

int pipe_action_child(Command **cmd, Redirection **redir, Pipeline **pipeline);
int common_pre_child(Redirection **redir);

int pipe_exec(ASTNode **node);
#endif