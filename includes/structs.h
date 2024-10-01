/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/10/01 20:03:49 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// #define REDIR_IN "<"
// #define REDIR_OUT ">"
// #define REDIR_APPEND ">>"
// #define DOLLAR "$"
// #define HEREDOC "<<"
// #define PIPE "|"
// #define SINGLE_Q "\'"
// #define DOUBLE_Q "\""

// 명령어 구조체 
typedef struct Command {
    char *cmd; // 명령어 이름 (예: "ls")
    char **env; //env execve()필요
    char **args;
    // int infile; // redirection <
    // int outfile; // redirection > >>
    // char *heredoc_limiter; 
    // char *herestring_str;
    // char *filename;
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
    int tmp_fd;
}   Pipeline;

// typedef struct token_malloc
// {
//     int bufsize;
//     int position;
//     int in_single_quote;
//     int in_double_quote;
//     int in_left_braket;
//     int in_right_braket;
//     char **new_token_list;
//     char *start;
//     char **token_list;
// } token_malloc;

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
    // NODE_HEREDOC, // <<
    // NODE_HERESTRING, //>>>
    // NODE_REDIRECTION, // > < >>
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

//이전 노드
    // typedef struct ASTNode
    // {
    //     NodeType type;         // 노드 유형 (명령어 또는 연산자)
    //     Command *command;      // 명령어가 있는 경우 (NODE_COMMAND에 사용)
    //     Pipeline    *pipe;     // 파이프가 있는 경우 (NODE_PIPE)
    //     char *heredoc_limiter; // heredoc limiter (NODE_HEREDOC)
    //     char *filename;        // 리디렉션에서 사용되는 파일명 (NODE_REDIRECTION에서 사용)
    //     char *dollar_vari;        // 변수 확장에서 사용되는 변수명 (NODE_DOLLAR에서 사용)
    //     struct ASTNode *left;  // 좌측 자식 (좌측 명령어 또는 서브트리) // 사실 이건 and 또는 or 연산자 있을때 우선순위를 위해서 사용.
    //     struct ASTNode *right; // 우측 자식 (우측 명령어 또는 서브트리) //그냥 next로만 해도 되는데 일단.
    // } ASTNode;

    // typedef struct ASTlist
    // {
    //     ASTNode *head;
    //     ASTNode *tail;
    // }   ASTlist;

// // 환경변수 구조체
// typedef struct Env_var
// {
//     char *name;          // 환경변수 이름 (예: "PATH")
//     char *value;         // 환경변수 값 (예: "/usr/bin:/bin")
//     struct Env_var *next; // 연결 리스트로 다음 환경변수를 가리킴
// } Env_var;

// // 환경변수 리스트 구조체
// typedef struct Envvar_list
// {
//     Env_var *head; // 환경변수 리스트의 시작 포인터
// } EnvList;

#endif