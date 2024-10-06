/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/06 19:01:28 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct Command {
    char *cmd; // 명령어 이름 (예: "ls")
    char **env; //env execve()필요
    char **args;
    int  exitcode;
    int  wstatus;
    // char *dollar_vari; // 변수 확장에서 사용되는 변수명 (TYPE_DOLLAR에서 사용)
}   Command;


//tokenize check_set
typedef struct s_set
{
    int depth;
    int single_quote;
    int double_quote;
    char *tmp_start;
    char *tmp_end;
}   t_set;
//heredoc read_line
typedef struct s_line
{
    char c;
    int r_byte;
    int i;
    int buf_size;
} t_line;

// //파이프 구조체
typedef struct Pipeline
{
    int fd[2];
    pid_t pid;
    pid_t left_pid;
    pid_t right_pid;
    int tmp_fd;
}   Pipeline;

//redirection type;
typedef enum 
{
    REDIRECT_INPUT,
    REDIRECT_OUTPUT,
    REDIRECT_APPEND
} REDIRECT;

// AST 노드 유형 정의
typedef enum
{
    NODE_COMMAND, // 명령어 노드
    // NODE_DOLLAR, // 확장변수
    NODE_PIPE,    // 파이프 연산자 노드
    NODE_SUBSHELL, // Subshell (()) 노드
    // NODE_BACKGROUND, // &
    NODE_AND, // AND 연산자 (&&) 노드 //보너스니까 일단 제외
    NODE_OR       // OR 연산자 (||) 노드 // 보너스니까 일단 제외
    // NODE_SEQ, // 순차 실행 (;) 노드 // 이건 안해줘도 됌
} NodeType;

//redirect 
typedef struct s_redirection
{
    int infile;  // redirection <
    int tmp_infile;
    int outfile; // redirection > >>
    char *filename;
    int direction_type; // REDIRECT_OUTPUT, REDIRECT_APPEND, REDIRECT_INPUT 등
    char *heredoc_limiter;
    char *herestring_str;
    char *dollar_vari; // 변수 확장에서 사용되는 변수명 (TYPE_DOLLAR에서 사용)
} Redirection;

// AST 노드 구조체
typedef struct ASTNode
{
    NodeType type;         // 노드 유형 (명령어 또는 연산자)
    Command *command;      // 명령어가 있는 경우 (NODE_COMMAND에 사용)
    Redirection *redir; 
    Pipeline    *pipeline;
    struct ASTNode *left;  // 좌측 자식 (좌측 명령어 또는 서브트리)
    struct ASTNode *right; // 우측 자식 (우측 명령어 또는 서브트리)
} ASTNode;

// for expand
typedef struct s_dollar
{
	size_t	len;
	size_t	out_len;
	char	*output;
	size_t	i;
	size_t	out_i;
	size_t	var_len;
	size_t	var_start;
	char	*var;
	char	*var_value;
}	t_dollar;

#endif