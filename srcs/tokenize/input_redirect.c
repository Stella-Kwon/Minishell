/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:37:42 by suminkwon         #+#    #+#             */
/*   Updated: 2024/08/31 19:06:24 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_redirection(Command **node, char *token, char *token_next)
{
    if (ft_strncmp(token, REDIR_IN, ft_strlen(REDIR_IN) == 0)) // Check for '<'
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
    // else if (ft_strncmp(*token, REDIR_OUT, ft_strlen(REDIR_OUT) == 0) // Check for '<'
    // {
        
    // }
    return (SUCCESS);
}


int redirect_in_action(Command **node, char *token, char *token_next)
{
    (*node)->flag = 0;
    if (*(token + 1) == '\0')
    {
        if (!token_next)
        {
            ft_putstr_fd("bash: ", 2);
            ft_putstr_fd("syntax error near unexpected token `newline\'", 2);
            ft_putstr_fd("\n", 2);
            (*node)->exitcode = 258; // 하면 안되고 ㅠ 따로 처리..
        }
        token = token_next; // 다음 노드인 token으로 넘겨주기 infile받아야지
        printf("token < infile : %s\n", token);
        
    } 
    else if (*(token + 1))
    {
        token++; // 바로 파일이름이 스페이스없이  <infile나왔을떄
        (*node)->flag = 1;
        printf("token <infile : %s\n", token);
    }
    if (token)
    {
        (*node)->infile = open_file(token, 0, 0);
        if ((*node)->infile == -1)
            return(log_errors("node->infile failed open"));
    }
    return (SUCCESS);
}

int check_heredoc(Command **node, char *token, char *token_next)
{
    (*node)->flag = 0;
    if (*(token + 1) && ft_strncmp(*(token + 1), REDIR_IN, ft_strlen(REDIR_IN)) == 0) // Check for "<<" by looking ahead
    {
        if (*(token + 2) == '\0')
        {
            //이걸 여기서 핸들링해야하는지 아니면 | 할때 
            // if (*(token_next) == '|' && ft_strlen(token_next) == 1)
            // {
                
            // }
            token = token_next; // 다음 노드인 token으로 넘어가기
        }
            
        else
        {
            token ++;
            // 스페이스 없이 input들어왔을때 <<limiter : 더블포인터라서 "<<" delimeter 다음의 위치에 있는 곳으로 넘겨준다
            (*node)->flag = 1;
        }
        // delimiter 넣어야지
        if (token)
        {
            (*node)->heredoc_limiter = ft_strdup(token);
            if (!(*node)->heredoc_limiter)
                return (log_errors("limiter failed to storing data in Command node"));
        }
        if ((*node)->flag == 1)
            *token++; // 다음 노드인 token으로 넘어가기
        return (TRUE);
    }
    return (FALSE);
}