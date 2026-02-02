./no_such_command
echo "data" > /tmp/minishell_no_exec
/bin/chmod 644 /tmp/minishell_no_exec
/tmp/minishell_no_exec
/bin/mkdir -p /tmp/minishell_exec_dir
/bin/chmod 755 /tmp/minishell_exec_dir
/tmp/minishell_exec_dir
echo test
/bin/rm -f /tmp/minishell_no_exec
/bin/rm -rf /tmp/minishell_exec_dir
exit
