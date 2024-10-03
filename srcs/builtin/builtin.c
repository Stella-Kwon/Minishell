#include "../../includes/minishell.h"

int builtin(Command *command) 
{
    if (ft_strncmp(command->cmd, "echo", 5) == 0)
        return echo(command->args, &command->env); //리턴 부분에서 cmd->args가 사용되는 이유는 echo와 같은 빌트인 함수가 해당 명령어에 대한 인자 목록을 필요로 하기 때문입니다. Command 구조체에서 args는 사용자가 입력한 명령어와 함께 제공된 인자(예: echo hello의 경우 "hello")를 포함하고 있습니다.
    else if (ft_strncmp(command->cmd, "cd", 3) == 0)
        return cd(command->args, &command->env);
    else if (ft_strncmp(command->cmd, "pwd", 4) == 0)
        return pwd(command->args, &command->env);
    else if (ft_strncmp(command->cmd, "export", 7) == 0)
        return export(command->args, &command->env);
    else if (ft_strncmp(command->cmd, "unset", 6) == 0)
        return unset(command->args, &command->env);
    else if (ft_strncmp(command->cmd, "env", 4) == 0)
        return env(command->args, &command->env);
    else if (ft_strncmp(command->cmd, "exit", 5) == 0)
        return mini_exit(command->args, &command->env);
    return (FAIL); // 빌트인 명령어가 아닐 경우
}

// int execute_builtin(Command *command, char ***envp_ptr) 
// {
//     if (builtin(command, envp_ptr) == SUCCESS)
//         return (SUCCESS);
//     return (FAIL);
// }

// int	builtin(char **cmd_argv, char ***envp_ptr)
// {
// 	if (ft_strncmp(cmd_argv[0], "echo", 5) == 0)
// 		return (echo(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "cd", 3) == 0)
// 		return (cd(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "pwd", 4) == 0)
// 		return (pwd(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "export", 7) == 0)
// 		return (export(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "unset", 6) == 0)
// 		return (unset(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "env", 4) == 0)
// 		return (env(cmd_argv, envp_ptr));
// 	else if (ft_strncmp(cmd_argv[0], "exit", 5) == 0)
// 		return (mini_exit(cmd_argv, envp_ptr));
// 	return (FAIL);
// }




