echo hello > /tmp/minishell_test_out
cat /tmp/minishell_test_out
echo world > /tmp/minishell_test_out
cat /tmp/minishell_test_out
echo first > /tmp/minishell_test_append
echo second >> /tmp/minishell_test_append
echo third >> /tmp/minishell_test_append
cat /tmp/minishell_test_append
echo "input data" > /tmp/minishell_test_in
cat < /tmp/minishell_test_in
cat < /tmp/minishell_test_in > /tmp/minishell_test_out2
cat /tmp/minishell_test_out2
echo test > /tmp/test1.txt
echo test2 > /tmp/test2.txt
cat < /tmp/test1.txt > /tmp/test3.txt
cat /tmp/test3.txt
cat < /tmp/minishell_test_in | wc -c
cat < /tmp/minishell_missing_file
rm -f /tmp/minishell_test_out /tmp/minishell_test_in /tmp/minishell_test_out2 /tmp/minishell_test_append
rm -f /tmp/test1.txt /tmp/test2.txt /tmp/test3.txt
exit
