/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suminkwon <suminkwon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 15:42:52 by suminkwon         #+#    #+#             */
/*   Updated: 2024/09/01 23:43:30 by suminkwon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REDIR_IN "<"
#define REDIR_OUT ">"
#define REDIR_APPEND ">>"
#define DOLLAR "$"
#define HEREDOC "<<"
#define PIPE "|"
#define SINGLE_Q "\'"
#define DOUBLE_Q "\""

// 명령어 구조체 
typedef struct Command {
    char    *cmd;            // 명령어 이름 (예: "ls")
    char    **args;  // 명령어 인자 (예: "-l") or extension '$HOME'과 같은  
    int    infile;  // 입력 리디렉션 파일fd
    int    outfile; // 출력 리디렉션 파일fd
    char    *heredoc_limiter;       //heredoc limiter
    int     flag;
    int     exitcode;
    // struct Command *next;  // 파이프 라인 다음 명령어 : ASTNODE써서 따로 필요없음
} Command;

// AST 노드 유형 정의
typedef enum
{
    NODE_COMMAND, // 단일 명령어 노드
    NODE_PIPE    // 파이프 연산자 노드
    // NODE_SEQ,     // 순차 실행 (;) 노드 // 이건 안해주기
    // NODE_AND,     // AND 연산자 (&&) 노드
    // NODE_OR       // OR 연산자 (||) 노드
} NodeType;

// AST 노드 구조체
typedef struct ASTNode
{
    NodeType type;         // 노드 유형 (명령어 또는 연산자)
    Command *command;      // 명령어가 있는 경우 (NODE_COMMAND에 사용)
    struct ASTNode *left;  // 좌측 자식 (좌측 명령어 또는 서브트리)
    struct ASTNode *right; // 우측 자식 (우측 명령어 또는 서브트리)
} ASTNode;

typedef struct ASTlist
{
    ASTNode node;
    struct ASTlist *next;
}   ASTlist;


// 환경변수 구조체
typedef struct Env_var
{
    char *name;          // 환경변수 이름 (예: "PATH")
    char *value;         // 환경변수 값 (예: "/usr/bin:/bin")
    struct Env_var *next; // 연결 리스트로 다음 환경변수를 가리킴
} Env_var;

// 환경변수 리스트 구조체
typedef struct Envvar_list
{
    Env_var *head; // 환경변수 리스트의 시작 포인터
} EnvList;

#endif