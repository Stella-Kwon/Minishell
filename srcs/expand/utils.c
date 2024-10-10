#include "../../includes/minishell.h"

char	*expand_cmd(char *cmd, char **env, int last_exitcode)
{
	return (find_dollar_signs(cmd, env, last_exitcode));
}

// 인자 확장 처리
char	**expand_args(char **args, char **env, int last_exitcode)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		args[i] = find_dollar_signs(args[i], env, last_exitcode);
		if (!args[i]) 
		{
            log_errors("Argument expansion failed", args[i]);
            return (NULL);
        }
		i++;
	}
	return (args);
}

int	expand_error(char *command)
{
	ft_putstr_fd("MINISHELL: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": bad substition", STDERR_FILENO);
	return (FAIL);
}

void	check_null_cmd(char *argv)
{
	int	len;
	int	i;
	int	found;

	found = 0;
	i = -1;
	len = ft_strlen(argv);
	while (++i < len)
	{
		if (argv[i] != ' ')
		{
			found = 1;
			break ;
		}
	}
	if (found == 0)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(argv, STDERR_FILENO);
		//error_exit(": command not found\n", 127);
	}
}