#include "../../includes/minishell.h"

char *expand_cmd(char *cmd, char **env, int last_exit_code)
{
    return (find_dollar_signs(cmd, env, last_exit_code));
}

// 인자 확장 처리
char **expand_args(char **args, char **env, int last_exit_code)
{
    int i;
	
	i = 0;
    while (args[i] != NULL) 
	{
        args[i] = find_dollar_signs(args[i], env, last_exit_code); // 각 인자에 대해 확장 처리
        i++; // 인덱스 증가
    }
    return (args);
}

int	expand_error(char *command)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": bad substition", STDERR_FILENO);
	return (FAIL);
}