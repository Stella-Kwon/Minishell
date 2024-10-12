/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlee-sun <hlee-sun@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:19:04 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/10 17:33:56 by hlee-sun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int common_pre_child(t_Redirection **redir, t_Command **cmd)
{
	if ((*redir)->infile != -2)
	{
		if ((*redir)->infile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd((*redir)->filename, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd("No such file or directory\n", 2);
			(*cmd)->exitcode = FAIL;
			exit(FAIL);
			// handle_error(cmd, (*redir)->filename);
			// exit((*cmd)->exitcode);
		}
		if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
		{
			log_errors("Failed to redirect infile", strerror(errno)); // 에러 메시지 추가
			return (FAIL);
		}
	}

	if ((*redir)->herestring_str)
	{
		if (here_string(redir) != SUCCESS)
			return (FAIL);
		if ((*redir)->infile != -2 &&
			dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
		{
			log_errors("Failed to redirect infile after herestring", strerror(errno)); // 에러 메시지 추가
			return (FAIL);
		}
	}
	return (SUCCESS);
}

int action_child(t_Command **cmd, t_Redirection **redir)
{
	if (common_pre_child(redir, cmd) == FAIL)
		exit(FAIL);
	if ((*redir)->outfile != -2)
	{
		if ((*redir)->outfile == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd((*redir)->filename, 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd("No such file or directory\n", 2);
			(*cmd)->exitcode = FAIL;
			exit(FAIL);
			// handle_error(cmd, (*redir)->filename);
			// exit((*cmd)->exitcode);
		}
		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
		{
			log_errors("Failed to redirect outfile in child process", strerror(errno)); // 에러 처리 추가
			(*cmd)->exitcode = FAIL;
			exit(FAIL);
		}
	}
	// ft_putstr_fd("before execution\n",2);
	execute_cmd(cmd);
	exit((*cmd)->exitcode);
}

// int	common_pre_child(t_Redirection	**redir)
// {
// 	if ((*redir)->infile != -2)
// 	{
// 		if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
// 			return (FAIL);
// 	}
// 	if ((*redir)->herestring_str)
// 	{
// 		if (here_string(redir) != SUCCESS)
// 			return (FAIL);
// 	if (dup_and_close((*redir)->infile, STDIN_FILENO) == FAIL)
// 		return (FAIL);
// 	}
// 	return (SUCCESS);
// }

// int	action_child(t_Command **cmd, t_Redirection **redir)
// {
// 	if (common_pre_child(redir) == FAIL)
// 		return (FAIL);
// 	if ((*redir)->outfile != -2)
// 		if (dup_and_close((*redir)->outfile, STDOUT_FILENO) == FAIL)
// 			return (FAIL);
// 	execute_cmd(cmd);
// 	exit((*cmd)->exitcode);
// }
