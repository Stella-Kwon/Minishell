#include "../../includes/minishell.h"

char	*remove_quotes(char *s)
{
	int		len;
	char	*new_s;

	len = ft_strlen(s);
	if ((s[0] == '"' && s[len - 1] == '"')
		|| (s[0] == '\'' && s[len - 1] == '\''))
	{
		new_s = malloc((len - 1) * sizeof(char));
		if (!new_s)
		{
			perror("MINISHELL: Memory allocation failed\n");
			free(s);
		}
		ft_strncpy(new_s, s + 1, len - 2);
		new_s[len - 2] = '\0';
		free(s);
		return (new_s);
	}
	return (s);
}

// void	check_path(char *path, char **cmd)
// {
// 	if (access(path, F_OK) == -1)
// 		cmd_error_exit(cmd, ": command not found\n", 127);
// 	if (access(path, X_OK) == -1)
// 		cmd_error_exit(cmd, ": Permission denied\n", 126);
// }

// void	check_cmd_script(char **s_cmd)
// {
// 	int	len;

// 	len = ft_strlen(s_cmd[0]);
// 	if (s_cmd[0][len - 3] == '.' &&
// 		s_cmd[0][len - 2] == 's' && s_cmd[0][len - 1] == 'h')
// 	{
// 		if (ft_strrchr(s_cmd[0], '/') == NULL)
// 		{
// 			cmd_error_exit(s_cmd, ": command not found\n", 127);
// 		}
// 		if (ft_strrchr(s_cmd[0], '/') != NULL)
// 			check_path(s_cmd[0], s_cmd);
// 	}
// }

// void	check_cmd_error(char **s_cmd)
// {
// 	int	len;

// 	if (!s_cmd || s_cmd[0] == NULL)
// 	{
// 		//free_arrays(s_cmd);
// 		cmd_error_exit(s_cmd, ": command not found\n", 127);
// 	}
// 	if (s_cmd[0][0] == '.' || s_cmd[0][0] == '/')
// 	{
// 		len = ft_strlen(s_cmd[0]);
// 		if (s_cmd[0][len - 1] == '/')
// 		{
// 			ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			ft_putstr_fd(s_cmd[0], STDERR_FILENO);
// 			//free_arrays(s_cmd);
// 			error_exit(" is a directory", 126);
// 		}
// 		if (access(s_cmd[0], F_OK) == -1)
// 			cmd_error_exit(s_cmd, ": No such file or directory\n", 127);
// 		if (access(s_cmd[0], X_OK) == -1)
// 			cmd_error_exit(s_cmd, ": Permission denied\n", 126);
// 	}
// }
// static int	check_quotes(const char *str, int i)
// {
// 	int	k;
// 	int	double_quotes;
// 	int	single_quotes;
// 	int	escape;

// 	double_quotes = 0;
// 	single_quotes = 0;
// 	escape = 0;
// 	k = 0;
// 	while (k <= i)
// 	{
// 		if (str[k] == '\\' && !escape)
// 			escape = 1;
// 		else
// 		{
// 			if (str[k] == '"' && !escape && !single_quotes)
// 				double_quotes = !double_quotes;
// 			if (str[k] == '\'' && !escape && !double_quotes)
// 				single_quotes = !single_quotes;
// 			escape = 0;
// 		}
// 		k++;
// 	}
// 	return (!double_quotes && !single_quotes);
// }