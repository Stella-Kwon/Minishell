#include "../../includes/minishell.h"

static bool	is_valid(char *var_name)
{
	size_t i;

	i = 1;
	if (!var_name || !(ft_isalpha(var_name[0]) || var_name[0] == '_'))
		return (false);
	while(var_name[i] != '\0')
	{
		if (!(ft_isalnum(var_name[i]) || var_name[i] == '_'))
			return (false);
	}

	return (true);
}

static int print_env_var(char *env_var)
{
    size_t i;

    if (!env_var)
        return (FAIL);
    i = 0;
    ft_putstr_fd("declare -x ", STDOUT_FILENO);
    while (env_var[i] != '\0')
    {
        ft_putchar_fd(env_var[i], STDOUT_FILENO);
        if (env_var[i] == '=')
            ft_putstr_fd("\"", STDOUT_FILENO);
        i++;
    }
    if (ft_strchr(env_var, '=') != NULL)
        ft_putstr_fd("\"", STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (SUCCESS);
}

static int print_sorted_envp(char **envp)
{
    size_t i;
    size_t len;
    char **sorted_envp;

    if (!envp)
        return (FAIL);
    len = get_str_len(envp);
    sorted_envp = ft_calloc(len + 1, sizeof(*sorted_envp));
    if (!sorted_envp)
    {
        perror("Error: malloc failed");
        return (FAIL);
    }
    if (copy_envp(envp, sorted_envp, len) == FAIL)
        return (FAIL);
    sort_envp(len, sorted_envp);
    i = 0;
    while (i < len)
    {
        if (sorted_envp[i][0] != '_' && sorted_envp[i][1] != '=')
        {
            if (print_env_var(sorted_envp[i]) == FAIL)
            {
                delete_str_array(&sorted_envp);
                return (FAIL);
            }
        }
        i++;
    }
    delete_str_array(&sorted_envp);
    return (SUCCESS);
}

void  export_check(char **args, char ***envp_ptr)
{
    if (args[1] == NULL)
    {
        print_sorted_envp(*envp_ptr);
        return ;
    }
    if (!is_valid(args[1])) // 변수명 유효성 검사
    {
        ft_putstr_fd("export: ", STDERR_FILENO);
        ft_putstr_fd(args[1], STDERR_FILENO);
        ft_putstr_fd(": invalid identifier\n", STDERR_FILENO);
        return ;
    }
    if (ft_strchr(args[1], '=') != NULL)
    {
        if (export(args, envp_ptr) == FAIL)
    		return ;
	}
}

/**
 * Processes export command when '=' is included in the argument.
 *
 * @param args The arguments passed to the export command.
 * @param envp_ptr A pointer to the environment variable array.
 * @return SUCCESS if the operation was successful, otherwise ERROR.
 */
int export(char **args, char ***envp_ptr)
{
    char    *var_name;
    char    *value;
    char    *tmp;
    size_t  i;
	
	i = 0;
    tmp = ft_strdup(args[1]);
    if (!tmp)
    {
        perror("Error: strdup failed");
        return (FAIL);
    }
    while (tmp[i] && tmp[i] != '=')
        i++;
    var_name = ft_substr(tmp, 0, i);
    value = ft_substr(tmp, i + 1, ft_strlen(tmp) - i);
    if (!var_name || !value)
    {
        perror("Error: substr failed");
        free(tmp);
        return (FAIL);
    }
    args[1] = var_name; // 이름만 사용하도록 수정
	unset(args, envp_ptr); // 기존 변수 제거
    append_to_env(ft_strjoin(var_name, "="), envp_ptr); // 새 변수 추가
    append_to_env(ft_strjoin(var_name, value), envp_ptr);
    //free(tmp);
    //free(var_name);
    //free(value);
    return (SUCCESS);
}
