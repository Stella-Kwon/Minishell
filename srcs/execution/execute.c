#include "../../includes/minishell.h"

static char	**find_env(char **envp)
{
	char	*path_var;

	path_var = NULL;
	if (*envp == NULL)
		return (ft_split("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", ':'));
	while (*envp)
	{
		if (ft_strncmp("PATH=", *envp, 5) == 0)
		{
			path_var = *envp + 5;
			break ;
		}
		envp++;
	}
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

static char	*get_path(Command *command, char **env_path)
{
	char	*temp;
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	if (!env_path)
		cmd_error(command, ": No such file or directory\n", 127);
	while (env_path[i])
	{
		temp = ft_strjoin(env_path[i], "/");
		path = ft_strjoin(temp, command->cmd);
		if (temp)
			free(temp);
		if (access(path, F_OK) == 0)
			break ;
		if (path)
			free(path);
		path = NULL;
		i++;
	}
	return (path);
}

static char	*find_and_check_path(Command *command) 
{
    char	**env_path;
    char	*path;

    if (ft_strrchr(command->cmd, '/') != NULL)
        return (command->cmd);
    env_path = find_env(command->env);
    path = get_path(command, env_path);
	{
		if (env_path != NULL)
			free_arrays(env_path);
	}
    if (!path)
		return (NULL);
    check_path(path, command);
    return (path);
}

int	execute_cmd(Command *command)
{
	char	*path;

	command->cmd = expand_cmd(command->cmd, command->env);
	command->args = expand_args(command->args, command->env);
	if (!command->cmd)
		return (cmd_error(command, ": command not found\n", 127));
	if (builtin(command) == SUCCESS)
		return (SUCCESS);
	if (check_cmd_script(command) == FAIL || check_cmd_error(command) == FAIL)
		return (FAIL);
	path = find_and_check_path(command);
	if (!path)
		return (cmd_error(command, ": command not found\n", 127));
	if (execve(path, command->args, command->env) == -1)
	{
		if (path != command->cmd)
			free(path);
		return (cmd_error(command, ": command not found\n", 127));
	}
	if (path != command ->cmd)
		free(path);
	// 절대 경로는 command->cmd가 프로그램 내에서 이미 관리되고 있는 문자열이므로, 이를 free하면 안 됩니다. 
	// 그러나 동적으로 생성된 상대 경로(혹은 PATH에서 찾은 경로)는 사용 후 반드시 해제해야 메모리 누수를 방지할 수 있습니다.
	command->exitcode = 0;
	return (SUCCESS);
}

