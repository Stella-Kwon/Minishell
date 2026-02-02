echo test | cat
echo data | cat | cat | cat | cat | wc -c
echo "stress test" | cat | cat | cat | cat | cat | cat | cat | cat | cat | wc -c
cat << EOF | cat
heredoc line 1
heredoc line 2
EOF
cat << EOF1 | cat << EOF2 | cat
first
EOF1
second
EOF2
cat << DATA | cat | cat | cat | wc -l
line1
line2
line3
DATA
echo test > /tmp/fd_test.txt
cat < /tmp/fd_test.txt | cat | wc -c
cat << EOF | grep test | cat > /tmp/fd_test2.txt
test line
other
EOF
cat /tmp/fd_test2.txt
cat << D1 | cat << D2 | cat << D3 | cat | cat | wc -l > /tmp/stress.txt && cat /tmp/stress.txt
data1
D1
data2
D2
data3
D3
rm -f /tmp/fd_test.txt /tmp/fd_test2.txt /tmp/stress.txt
exit
