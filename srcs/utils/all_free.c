#include "minishell.h"

char	**all_free(char ***res)
{
	int	i;

	i = 0;
	while ((*res)[i])
	{
		free((*res)[i]);
		(*res)[i] = NULL;
		i++;
	}
	free(*res);
	*res = NULL;
	return (NULL);
}