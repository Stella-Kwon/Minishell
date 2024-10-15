/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_filename.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:21:10 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/13 23:42:32 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int filename_restore(int *i, char ***args, char **new_filename, int *buffersize)
{
    int j;

    j = 0;
    while ((**args)[*i] && *i < (int)ft_strlen(**args) - 1) // " 끝제외하고
    {
        if ((**args)[*i] == '"' || (**args)[*i] == '\'')
        {
            (*i)++;
            continue;
        }
        (*new_filename)[j] = (**args)[*i];
        (*i)++;
        j++;
        *new_filename = ft_realloc_single(*new_filename, j, buffersize);
        if (!new_filename)
            return (log_errors("Failed to remalloc in filename_with_quote", ""));
        if (!(**args)[*i])
            break;
    }
    (*new_filename)[j] = '\0';
    return (SUCCESS);
}

int pre_action_rm_quote(t_Redirection **redirect, char ***args, char **filename)
{
    if ((*redirect)->infile != -1 && (*redirect)->outfile != -1)
    {
        (*filename) = ft_strdup(**args);
        if (!(*filename))
            return (log_errors("Failed in storing filename in set_redirection", ""));
    }
    if ((*filename) && (*filename)[0] != '"' && (*filename)[0] != '\'')
        return (SUCCESS);
    return (SUCCESS);
}

int rm_quote_filename(t_Redirection **redirect, char ***args, char **filename)
{
    char *new_filename;
    int i;
    int buffersize;

    i = 0;
    buffersize = BUFFER_SIZE;
    new_filename = NULL;

    if (pre_action_rm_quote(redirect, args, filename) != SUCCESS)
        return (FAIL);
    if ((**args)[i] == '"' || (**args)[i] == '\'') // 나중에 " "set맞출때는 여기다가 readline다시 열어주면됌.
    {
        i++;
        new_filename = malloc(buffersize * sizeof(char));
        if (!new_filename)
            return (log_errors("Failed to malloc new_filename in rm_quote_filename", ""));
        if (filename_restore(&i, args, &new_filename, &buffersize) != SUCCESS)
            return (log_errors("Failed to filename_restore in rm_quote_filename", ""));
        if ((*filename))
            free_one((void **)&(*filename));
        (*filename) = new_filename;
    }
    return (SUCCESS);
}