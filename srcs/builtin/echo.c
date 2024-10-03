#include "../../includes/minishell.h"

static int	is_n(char *str)
{
	size_t	i;

	if (str[0] == '-' && str[1] == 'n')
	{
		i = 2;
		while (str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (true);
	}
	return (false);
}

static void	print_args(char **args, size_t i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	echo(char **args, char ***envp_ptr)
{
	int		n_option;

	(void)envp_ptr;
	n_option = false;
	if (args[1] != NULL)
	{
		if (is_n(args[1]) == true)
			n_option = true;
		print_args(args, 1 + n_option);
	}
	if (!n_option)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
