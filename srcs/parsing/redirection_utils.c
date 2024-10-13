/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 03:42:27 by sukwon            #+#    #+#             */
/*   Updated: 2024/10/12 00:57:56 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int filename_restore(int *i, char ***args, char **new_filename, int *buffersize)
{
    (void)buffersize;
    int j;

    j = 0;

    // printf("i : %d\n", *i);
    // printf("(**args)[i] : %c\n", (**args)[*i]);

    while ((**args)[*i] && *i < (int)ft_strlen(**args))
    {
        if ((**args)[*i] == '"' || (**args)[*i] == '\'')
        {
            (*i)++;
            continue;
        }
        // printf("(**args)[i] : %c\n", (**args)[*i]);
        (*new_filename)[j] = (**args)[*i];
        // printf("*new_filename[j] : %c\n", (*new_filename)[j]);
        (*i)++;
        j++;
        *new_filename = ft_realloc_single(*new_filename, j, buffersize);
        if (!new_filename)
            return (log_errors("Failed to remalloc in filename_with_quote", ""));
        if (!(**args)[*i])
            break;
    }
    (*new_filename)[j] = '\0';
    // printf("*new_filename : %sword\n", (*new_filename));
    return (SUCCESS);
}

int rm_quote_filename(t_Redirection **redirect, char ***args)
{
    char *new_filename;
    int i;
    int buffersize;

    i = 0;
    buffersize = BUFFER_SIZE;
    new_filename = NULL;
    if ((*redirect)->filename && (*redirect)->filename[0] != '"' && (*redirect)->filename[0] != '\'')
        return (SUCCESS);
    if ((**args)[i] == '"' || (**args)[i] == '\'') // 나중에 " "set맞출때는 여기다가 readline다시 열어주면됌.
    {
        i++;
        // printf("(**args)[i] : %c\n", (**args)[i]);
        // if ((**args)[i] == '.' && (**args)[i + 1] == '/')
        // 	i += 2;
        new_filename = malloc(buffersize * sizeof(char));
        if (!new_filename)
            return (log_errors("Failed to malloc new_filename in rm_quote_filename", ""));
        if (filename_restore(&i, args, &new_filename, &buffersize) != SUCCESS)
            return (log_errors("Failed to filename_restore in rm_quote_filename", ""));
        if ((*redirect)->filename)
            free_one((void **)&(*redirect)->filename);
        (*redirect)->filename = new_filename;
    }
    return (SUCCESS);
}

void check_directory_exist(t_Redirection **redirect, char *filename)
{
    char **split_filename;
    int i;
    int count;

    count = 0;
    i = 0;

    split_filename = ft_split(filename, '/');
    while (split_filename[count])
        count++;
    while (split_filename[i] && i < count)
    {
        if (access(filename, F_OK) != 0)
        {
            (*redirect)->outfile = open(filename, O_WRONLY | O_TRUNC, 0644);
            return;
        }
        i++;
    }
    (*redirect)->outfile = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}
