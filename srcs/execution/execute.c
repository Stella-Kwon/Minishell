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

static char	*get_path(Command **command, char **env_path)
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
		path = ft_strjoin(temp, (*command)->cmd);
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

static char	*find_and_check_path(Command **command) 
{
    char	**env_path;
    char	*path;

    if (ft_strrchr((*command)->cmd, '/') != NULL)
        return ((*command)->cmd);
    env_path = find_env((*command)->env);
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

int	execute_cmd(Command **command)
{
	char	*path;

	if (!command || !*command)
		return (SUCCESS);
	(*command)->cmd = expand_cmd((*command)->cmd, (*command)->env);
	(*command)->args = expand_args((*command)->args, (*command)->env);

	if (!(*command)->cmd)
		return (cmd_error(command, ": command not found\n", 127));

	if (builtin(*command) == SUCCESS)
	{
		printf("success in builtin\n");
		(*command)->exitcode = 0;
		return (SUCCESS);
	}

	if (check_cmd_script(command) == FAIL || check_cmd_error(command) == FAIL)
		return (FAIL);

	path = find_and_check_path(command);
	if (!path)
		return (cmd_error(command, ": command not found\n", 127));

	// char **args = (*command)->args; // 원래의 args를 저장
	// while (*args)
	// {
		// printf("cmd->args : %s\n", *args);
		// args++; // args 포인터를 증가시킴
	// }
	// if (!*args)
		// printf("no args\n");
	// printf("path : %s\n", path);
	execve(path, (*command)->args, (*command)->env);
	if (path != (*command)->cmd)
		free_one((void **)&path);
	(*command)->exitcode  = 127;
	return ((*command)->exitcode);
}

