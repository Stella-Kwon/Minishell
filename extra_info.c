
// exec_command_with_env 함수 수정
int exec_command_with_env(t_Command *command, char **env, int last_exitcode)
{
    command->cmd = expand_cmd(command->cmd, env, last_exitcode);
    if (!command->cmd)
    {
        command->exitcode = 1;
        return command->exitcode;
    }

    command->args = expand_args(command->args, env, last_exitcode);
    if (!command->args)
    {
        command->exitcode = 1;
        return command->exitcode;
    }

    if (strncmp(command->cmd, "export", 6) == 0 && command->args[1])
    {
        char *var = command->args[1];
        char *key = strtok(var, "=");
        char *value = strtok(NULL, "=");

        if (key && value)
        {
            setenv(key, value, 1);
        }
    }

    // execve 실행
    if (execve(command->cmd, command->args, env) == -1)
    {
        perror("minishell: execve failed");
        command->exitcode = 1;  // 실패 시 exitcode 업데이트
        return command->exitcode;
    }

    // 성공적으로 실행되면 command->exitcode는 0
    return 0;
}

// handle_redirection 함수 수정
int handle_redirection(t_Redirection *redir)
{
    // 입력 리디렉션 처리
    if (redir->infile > 0)
    {
        if (dup2(redir->infile, STDIN_FILENO) == -1)
        {
            perror("dup2 input redirection failed");
            return FAIL; // 실패 시 오류 코드 반환
        }
        close(redir->infile); // 불필요한 파일 디스크립터 닫기
    }

    // 출력 리디렉션 처리
    if (redir->outfile > 0)
    {
        if (redir->direction_type == REDIRECT_OUTPUT)
        {
            // 기존 파일 내용을 덮어씀
            if (dup2(redir->outfile, STDOUT_FILENO) == -1)
            {
                perror("dup2 output redirection failed");
                return FAIL; // 실패 시 오류 코드 반환
            }
        }
        else if (redir->direction_type == REDIRECT_APPEND)
        {
            // 파일에 내용을 추가
            if (dup2(redir->outfile, STDOUT_FILENO) == -1)
            {
                perror("dup2 append redirection failed");
                return FAIL; // 실패 시 오류 코드 반환
            }
        }
        close(redir->outfile); // 불필요한 파일 디스크립터 닫기
    }

    // heredoc 처리 (필요 시 추가 구현 필요)
    // 만약 heredoc이 사용되었다면 처리 로직을 추가합니다.

    return SUCCESS; // 성공 시 성공 코드 반환
}


42서울(또는 42스쿨)의 minishell 프로젝트에서는 Bash와 유사한 셸을 구현하는 것이 목표이므로, Bash에서 발생하는 에러 메시지와 종료 코드를 정확하게 처리하는 것이 중요합니다. 여기에서는 Bash에서 발생하는 주요 에러 메시지와 관련된 종료 코드들을 정리하여 제공합니다.

주요 에러 메시지와 종료 코드
Permission denied (퍼미션 거부):

에러 메시지: bash: <파일 경로>: Permission denied
설명: 실행하려는 파일이 실행 권한이 없을 때 발생합니다.
종료 코드: 126
Command not found (명령어를 찾을 수 없음):

에러 메시지: bash: <명령어>: command not found
설명: 존재하지 않는 명령어를 입력했을 때 발생합니다.
종료 코드: 127
Is a directory (디렉토리 오류):

에러 메시지: bash: <경로>: Is a directory
설명: 파일이 아닌 디렉토리를 실행하려고 시도할 때 발생합니다.
종료 코드: 126
No such file or directory (파일 또는 디렉토리 없음):

에러 메시지: bash: <경로>: No such file or directory
설명: 지정된 경로의 파일 또는 디렉토리가 존재하지 않을 때 발생합니다.
종료 코드: 127
Not a directory (디렉토리가 아님):

에러 메시지: bash: cd: <경로>: Not a directory
설명: cd 명령어로 이동하려는 경로가 파일일 경우 발생합니다.
종료 코드: 1
File name too long (파일 이름이 너무 길 때):

에러 메시지: bash: <파일명>: File name too long
설명: 파일 이름이 시스템에서 허용하는 최대 길이를 초과했을 때 발생합니다.
종료 코드: 1
Segmentation fault (세그멘테이션 오류):

에러 메시지: Segmentation fault (core dumped)
설명: 프로그램이 메모리에 잘못된 접근을 했을 때 발생합니다.
종료 코드: 139
Illegal option (잘못된 옵션):

에러 메시지: bash: <명령어>: -<옵션>: invalid option
설명: 지원하지 않는 옵션을 사용했을 때 발생합니다.
종료 코드: 2
Syntax error (문법 오류):

에러 메시지: bash: syntax error near unexpected token '<token>'
설명: 잘못된 구문을 사용했을 때 발생합니다.
종료 코드: 2
Argument list too long (인자 목록이 너무 길 때):

에러 메시지: bash: /bin/ls: Argument list too long
설명: 시스템에서 허용하는 최대 인자 목록의 길이를 초과했을 때 발생합니다.
종료 코드: 1
Bad file descriptor (잘못된 파일 디스크립터):

에러 메시지: bash: <경로>: Bad file descriptor
설명: 존재하지 않는 파일 디스크립터를 참조했을 때 발생합니다.
종료 코드: 1
Floating point exception (부동 소수점 예외):

에러 메시지: Floating point exception (core dumped)
설명: 0으로 나누기와 같은 잘못된 부동 소수점 연산을 시도했을 때 발생합니다.
종료 코드: 136
Interrupted system call (중단된 시스템 호출):

에러 메시지: bash: <경로>: Interrupted system call
설명: 시스템 호출이 신호에 의해 중단되었을 때 발생합니다.
종료 코드: 1
Directory not empty (디렉토리가 비어있지 않음):

에러 메시지: bash: <경로>: Directory not empty
설명: rmdir 명령어를 사용하여 비어있지 않은 디렉토리를 삭제하려고 할 때 발생합니다.
종료 코드: 1
Bus error (버스 에러):

에러 메시지: Bus error (core dumped)
설명: 메모리 접근 관련 심각한 오류가 발생했을 때 발생합니다.
종료 코드: 135
Too many symbolic links (심볼릭 링크가 너무 많음):

에러 메시지: bash: <경로>: Too many levels of symbolic links
설명: 심볼릭 링크의 중첩이 시스템에서 허용하는 최대 레벨을 초과했을 때 발생합니다.
종료 코드: 1
Bash 기본 종료 코드
0: 성공적인 종료 (정상적인 명령어 실행)
1: 일반적인 오류 (그 외의 다양한 오류 상황)
2: 불법적인 명령어 사용 (예: 잘못된 명령어 구문, 잘못된 옵션 사용)
126: 명령어가 실행할 수 없는 파일일 때 (예: 실행 권한이 없는 파일을 실행하려고 할 때)
127: 명령어를 찾을 수 없을 때 (예: 존재하지 않는 명령어)
128+x: 신호 x에 의해 종료되었을 때. 예를 들어, 139는 SIGSEGV(세그멘테이션 오류)로 인해 종료된 경우입니다.
130: 프로그램이 Ctrl+C (SIGINT)로 종료되었을 때
이 메시지들과 종료 코드를 정확히 처리하면, Bash와 동일하게 에러 핸들링을 할 수 있을 것입니다.





이 코드도 위에 static int	process_character(char *input, t_Dollar *dol, char **env, int last_exitcode) 와 동일한 목적과 방법으로 두개로 나눠줄래? 예를 들면 밑에처럼, 정말 딱 두개로 나눠줘. 
#include <stdlib.h>
#include <string.h>

// 기존 구조체 및 함수 선언이 여기 있어야 합니다.
// 예: t_Dollar, handle_single_quote, handle_double_quote 등

static int handle_special_character(char *input, t_Dollar *dol, char **env, int last_exitcode) {
    if (input[dol->i] == '\'') {
        if (handle_single_quote(input, dol) == FAIL)
            return (FAIL);
    }
    else if (input[dol->i] == '\"') {
        if (handle_double_quote(input, dol, env, last_exitcode) == FAIL)
            return (FAIL);
    }
    else if (input[dol->i] == '$') {
        if (for_dollar_sign(input, dol, env, last_exitcode) == FAIL) {
            expand_error(input);
            return (FAIL);
        }
    }
    return (SUCCESS);
}

static int process_character(char *input, t_Dollar *dol, char **env, int last_exitcode) {
    // 특수 문자 처리
    if (handle_special_character(input, dol, env, last_exitcode) == FAIL)
        return (FAIL);

    // 일반 문자 처리
    dol->output[dol->out_i++] = input[dol->i++];
    if (dol->out_i >= dol->out_len - 1) {
        dol->output = ft_realloc_single(dol->output, dol->out_i, (int *)&dol->out_len);
        if (!dol->output) {
            log_errors("Realloc failed", "");
            return (FAIL);
        }
    }
    return (SUCCESS);
}