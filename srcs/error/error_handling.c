/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skwon2 <skwon2@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 12:50:33 by skwon2            #+#    #+#             */
/*   Updated: 2026/02/18 12:50:33 by skwon2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_258_exitcode_print(char *msg)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd("syntax error near unexpected token ", 2);
    ft_putstr_fd(msg, 2);
    ft_putstr_fd("\n", 2);
    return (2);
}

int check_specific_error(char *path, int ernbr)
{
    if (ernbr == EACCES)
    {
        struct stat st;
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
            ft_putstr_fd("is a directory\n", STDERR_FILENO);
        else
            ft_putstr_fd("Permission denied\n", STDERR_FILENO);
        return (126);
    }
    else if (ernbr == ENOENT)
    {
        ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
        return (127);
    }
    else if (ernbr == EISDIR)
    {
        ft_putstr_fd("is a directory\n", STDERR_FILENO);
        return (126);
    }
    else
    {
        ft_putstr_fd("Unknown error\n", STDERR_FILENO);
        return (1);
    }
}

int handle_error(char *path)
{
    int len;

    len = ft_strlen(path);
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(path, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    if (path[len - 1] == '/')
    {
        ft_putstr_fd("is a directory", STDERR_FILENO);
        return (126);
    }
    return (check_specific_error(path, errno));
}
