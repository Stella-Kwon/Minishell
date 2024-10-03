#include "../../includes/minishell.h"

int	pwd(char **args, char ***envp_ptr)
{
	char	*buf;

	(void)args;
	(void)envp_ptr;
	buf = ft_calloc(1, MAXPATHLEN);
	if (buf == NULL)
	{
		perror("Error: malloc failed");
		return (FAIL);
	}
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		perror("Error: getcwd failed");
		free(buf);
		return (FAIL);
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(buf);
	return (SUCCESS);
}
