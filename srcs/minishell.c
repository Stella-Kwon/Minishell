#include "../includes/minishell.h"

int main(int argc, char **argv, char **env)
{
	char *input;
	char **tokens;
	t_ASTNode *root;
	int last_exitcode;
	struct termios term;

	(void)argv;
	(void)env;
	(void)root;
	if (!isatty(STDOUT_FILENO))
	{
	signal(SIGTTIN, SIG_IGN); // SIGTTIN을 무시
	signal(SIGTTOU, SIG_IGN); // SIGTTOU를 무시
	} 
	else 
	{
    // 터미널에서 실행 중이면, 필요한 시스템 호출을 처리
		if (tcgetattr(STDIN_FILENO, &term) == -1) 
			perror("tcgetattr");
	}
	if (argc != 1)
		return (1);
	last_exitcode = 0;
	set_ehcoctl(1);
	while (1)
	{
		init_signal();
		input = readline("MINISHELL > ");
		if (!input)
		{
			ft_putstr_fd("\033[A",1);
			ft_putstr_fd("\033[K",1);
			ft_putstr_fd("\033[1G",1);
			ft_putstr_fd("MINISHELL > exit\n", 2);
			exit(0);
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		tokens = tokenize_input(&input);
		if (!tokens)
		{
			fprintf(stderr, "Error: Tokenization failed.\n");
			free(input);
			continue ;
		}
		root = parse_to_nodes(tokens, env);
		if (!root)
		{
			fprintf(stderr, "Error: Parsing failed.\n");
			free(input);
			free(tokens);
			continue ;
		}
		init_execution_signal();
		set_last_exitcode(&root, last_exitcode);
		ast_node_execution(&root);
		get_last_exitcode(&root, &last_exitcode);
		free_astnode(&root);
		free(input);
		free(tokens);
	}
	return (0);
}
