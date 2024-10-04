#include "../../includes/minishell.h"

char *expand_cmd(char *cmd, char **env)
{
    return (find_dollar_signs(cmd, env)); // 달러 기호로 시작하는 변수를 확장
}

// 인자 확장 처리
char **expand_args(char **args, char **env)
{
    int i = 0; // 인덱스 초기화
    while (args[i] != NULL) { // args가 NULL이 아닐 때까지 반복
        args[i] = find_dollar_signs(args[i], env); // 각 인자에 대해 확장 처리
        i++; // 인덱스 증가
    }
    return args;
}