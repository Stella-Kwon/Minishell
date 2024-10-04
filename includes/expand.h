#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"

char	*find_dollar_signs(char *input, char **envp);
char	*expand_cmd(char *cmd, char **env);
char	**expand_args(char **args, char **env);

#endif