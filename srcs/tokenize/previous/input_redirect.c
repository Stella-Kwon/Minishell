/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:37:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/03 15:38:05 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int error_input_258_handling(Command **node, char *token, char *token_next)
{
    if (!token_next && (ft_strncmp(token, "<|", 2) == 0 ||
    ft_strncmp(token, "<<|", 3) == 0 || ft_strncmp(token, "<<<|", 4) == 0))
        handle_258_exitcode_print("|");
    else if ((!token_next && (ft_strncmp(token, "<", 1) == 0 || \
    ft_strncmp(token, "<<", 2) == 0 || ft_strncmp(token, "<<<", 3) == 0)))
        handle_258_exitcode_print("`newline'");
    else if (!token_next && *token == REDIR_IN && *(token + 1) == REDIR_IN && *(token + 2) == REDIR_IN && *(token + 3))
    {
        if (*(token + 3) == REDIR_IN)
        {
            if (*(token + 4) == REDIR_IN)
            {
                if(*(token + 5) == REDIR_IN)
                    handle_258_exitcode_print("<<<");
                handle_258_exitcode_print("<<");
            }
            handle_258_exitcode_print("<");
        }
        return (0);
    }
    (*node)->exitcode = 258;
    return ((*node)->exitcode);
}

int redirect_in_action(Command **node, char *token, char *token_next)
{
    char *infile;

    infile = NULL;
    if (*(token + 1) == '\0')
    {
        token = token_next; // 다음 노드인 token으로 넘겨주기 infile받아야지
        printf("token < infile : %s\n", token);
        
    } 
    else if (*(token + 1))
    {
        token++; // 바로 파일이름이 스페이스없이  <infile나왔을떄
        printf("token <infile : %s\n", token);
    }
    if (token)
    {
        infile = ft_strdup(token);
        (*node)->infile = open_file(infile, 0, 0);
        free(infile);
        if ((*node)->infile == -1)
            return(log_errors("node->infile failed open"));
    }
    return (SUCCESS);
}

int check_heredoc(Command **node, char *token, char *token_next)
{
    if (*(token + 1) && *(token + 1) == REDIR_IN) // Check for "<<" by looking ahead
    {
        if (*(token + 2) == '\0')
            token = token_next; // 다음 노드인 token으로 넘어가기
        else
            token++;
        // delimiter 넣어야지
        if (token)
        {
            printf("token heredoc : %s\n", token);
            (*node)->heredoc_limiter = ft_strdup(token);
            if (!(*node)->heredoc_limiter)
                return (log_errors("limiter failed to storing data in Command node"));
        }
        return (TRUE);
    }
    return (FALSE);
}

int check_redirection(Command **node, char *token, char *token_next)
{
    if (error_input_258_handling(node, token, token_next) == 258)
        return (258);
    if (*token == REDIR_IN) // Check for '<'
    {
        if (check_heredoc(node, token, token_next) == TRUE)
        {
            printf("execution heredoc\n");
            return (here_doc(node));
        }
        else
        {
            printf("execution infile\n");
            return (redirect_in_action(&node, token, token_next));
        }
    }
    else if (ft_strncmp(*token, REDIR_OUT, ft_strlen(REDIR_OUT) == 0)// Check for '<'
    {
        
    }
    return (SUCCESS);
}