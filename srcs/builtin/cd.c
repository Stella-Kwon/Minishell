#include "../../includes/minishell.h"

void	check_and_set_path(char **args, char ***envp_ptr)
{
	char	*path;
	char	*home;

	path = NULL;
	if (args[1][0] == '-' && args[1][1] == '\0')
	{
		// free(args[1]);
		path = get_env_value("OLDPWD", *envp_ptr);
		if (path == NULL)
		{
			perror("Error: OLDPWD not set");
			return ;
		}
		args[1] = ft_strdup(path);
		if (args[1] == NULL)
		{
			perror("Error: strdup failed");
			return ;
		}
	}
	else if (args[1][0] == '~' && (args[1][1] == '\0' || \
			args[1][1] == '/'))
	{
		path = get_env_value("HOME", *envp_ptr);
		if (path == NULL)
		{
			perror("Error: HOME not set");
			return ;
		}
		home = ft_strjoin(home, args[1] + 1);
		if (home == NULL)
		{
			perror("Error: strjoin failed");
			return ;
		}
		args[1] = home;
	}
}

static void	currdir_to_old_pwd(char **args, char ***envp_ptr)
{
	char	*here;
	char	*path;
	char	*tmp;

	here = ft_calloc(1, MAXPATHLEN);
	if (here == NULL)
	{
		perror("Error: calloc failed");
		return ;
	}
	if (getcwd(here, MAXPATHLEN) == NULL)
	{
		perror("Error: getcwd failed");
		free(here);
		return ;
	}
	path = ft_strjoin("OLDPWD=", here);
	free(here);
	if (path == NULL)
	{
		perror("Error: strjoin failed");
		return ;
	}
	tmp = args[1];
	args[1] = path;
	export(args, envp_ptr);
	free(path);
	args[1] = tmp; // 원래 args[1] 복원
}

static void	go_home(char ***envp_ptr)
{
	char	*path_old;
	char	*path_new;
	char	**args_new;

	if (chdir(get_env_value("HOME", *envp_ptr)) == SUCCESS)
	{
		args_new = ft_calloc(3, sizeof(*args_new));
		if (args_new == NULL)
		{
			perror("Error: calloc failed");
			return ;
		}
		args_new[0] = ft_strdup("export");
		if (args_new[0] == NULL)
		{
			perror("Error: strdup failed");
			free(args_new);
			return ;
		}
		path_old = ft_strjoin("PWD=", get_env_value("HOME", *envp_ptr));
		if (path_old == NULL)
		{
			perror("Error: strjoin failed");
			free(args_new[0]);
			free(args_new);
			return ;
		}
		path_new = ft_strjoin("OLDPWD=", get_env_value("HOME", *envp_ptr));
		if (path_new == NULL)
		{
			perror("Error: strjoin failed");
			free(path_old);
			free(args_new[0]);
			free(args_new);
			return ;
		}
		args_new[1] = path_old;
		export(args_new, envp_ptr);
		free(args_new[1]);
		args_new[1] = path_new;
		export(args_new, envp_ptr);
		free(args_new[0]);
		free(args_new[1]);
		free(args_new);
	}
}

static char	*get_current_dir(void)
{
	char	*path;
	char	*buf;

	buf = ft_calloc(1, MAXPATHLEN);
	if (buf == NULL)
	{
		perror("Error: calloc failed");
		return (NULL);
	}
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		perror("Error: getcwd failed");
		free(buf);
		return (NULL);
	}
	path = ft_strjoin("PWD=", buf);
	free(buf);
	if (path == NULL)
	{
		perror("Error: strjoin failed");
		return (NULL);
	}
	return (path);
}

int	cd(char **args, char ***envp_ptr)
{
	int		status;
	char 	*new_dir;

	status = SUCCESS;
	if (args[1] == NULL)
		go_home(envp_ptr);
	else
	{
		if (args[1][0] == '-' || args[1][0] == '~')
			check_and_set_path(args, envp_ptr);
		currdir_to_old_pwd(args, envp_ptr);
		if (chdir(args[1]) == SUCCESS)
		{
			new_dir = get_current_dir();
			if (new_dir == NULL)
				status = FAIL;
			else
			{
				//free(args[1]);
				args[1] = new_dir;
				export(args, envp_ptr);
			}
		}
		else
		{
			print_error_cd(args[1]);
			status = FAIL;
		}
	}
	return (status);
}
