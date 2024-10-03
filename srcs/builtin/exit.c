#include "../../includes/minishell.h"

static void print_exit_err_msg(char *arg)
{
	ft_putstr_fd("Error: minishell, exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("Error: invalid argument\n", STDERR_FILENO);
	exit (255);
}

int	mini_exit(char **args, char ***envp_ptr)
{
	int exit_status;
	size_t i;

	i = 0;
	(void)envp_ptr;
	if (args[1] == NULL)
		exit(0);
	while (args[1][i] != '\0')
	{
		if (!ft_isdigit(args[1][i]))
			print_exit_err_msg(args[1]);
		i++;
	}
	if (args[2] != NULL)
	{
		ft_putstr_fd("Error, exit, too many arguments\n", STDERR_FILENO);
		return FAIL;
	}
	exit_status = ft_atoi(args[1]); // 문자열을 숫자로 변환
	exit(exit_status % 256); // 종료 상태 코드가 0~255 범위에 있도록 처리
}
