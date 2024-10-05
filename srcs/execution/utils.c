#include "../../includes/minishell.h"

int	error_exitcode(t_Command *command, char *s, int error_nb)
{
	ft_putstr_fd(s, STDERR_FILENO);
	command->exitcode = error_nb;
	return (FAIL);
}

int	cmd_error(t_Command *command, char *s, int error_nb)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command->cmd, STDERR_FILENO);
	ft_putstr_fd(s, STDERR_FILENO);
	command->exitcode = error_nb;
	return (FAIL);
}

int	check_path(char *path, t_Command *command)
{
	if (access(path, F_OK) == -1)
		return (cmd_error(command, ": command not found\n", 127));
	if (access(path, X_OK) == -1)
		return (cmd_error(command, ": Permission denied\n", 126));
	return (SUCCESS);
}

int	check_cmd_script(t_Command *command)
{
	int	len;

	len = ft_strlen(command->cmd);
	if (command->cmd[len - 3] == '.' &&
		command->cmd[len - 2] == 's' && command->cmd[len - 1] == 'h')
	{
		if (ft_strrchr(command->cmd, '/') == NULL)
		{
			cmd_error(command, ": command not found\n", 127);
			return (FAIL);
		}
		check_path(command->cmd, command);
	}
	return (SUCCESS);
}

int	check_cmd_error(t_Command *command)
{
	int	len;

	if (command->cmd == NULL)
		return (cmd_error(command, ": command not found\n", 127));
	if (check_null_cmd(command) == FAIL)
		return (FAIL);
	if (command->cmd[0] == '.' || command->cmd[0] == '/')
	{
		len = ft_strlen(command->cmd);
		if (command->cmd[len - 1] == '/')
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(command->cmd, STDERR_FILENO);
			return (error_exitcode(command, " is a directory", 126));
		}
		if (access(command->cmd, F_OK) == -1)
			return (cmd_error(command, ": No such file or directory\n", 127));
		if (access(command->cmd, X_OK) == -1)
			return (cmd_error(command, ": Permission denied\n", 126));
	}
	return (SUCCESS);
}

int	check_null_cmd(t_Command *command)
{
	int	len;
	int	i;
	int	found;

	found = 0;
	i = -1;
	len = ft_strlen(command->cmd);
	while (++i < len)
	{
		if (command->cmd[i] != ' ')
		{
			found = 1;
			break ;
		}
	}
	if (found == 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command->cmd, STDERR_FILENO);
		cmd_error(command, ": command not found\n", 127);
		return (FAIL);
	}
	return (SUCCESS);
}

void	free_arrays(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}