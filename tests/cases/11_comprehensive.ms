export TEST_VAR=integration_test
export NUM=42
cat << EOF | grep success | wc -l > /tmp/comp_out && cat /tmp/comp_out || echo fail
failure line
success line 1
another failure
success line 2
EOF
echo "User: $USER
Test: $TEST_VAR
Number: $NUM" | grep Test | wc -l
true && echo "step1" || echo "skip" && echo "step2"
false || echo "recover1" && echo "continue1" || echo "skip"
cat << DATA | grep "$USER" | wc -l | cat
test line
username: $USER
another: $TEST_VAR
DATA
cat << EOF1 | cat << EOF2 | cat
first heredoc
EOF1
second heredoc
EOF2
echo "test data" > /tmp/comp1.txt && cat < /tmp/comp1.txt | wc -c || echo fail
echo "multi
line
echo" | wc -l
cat << 'NOQUOTE' | cat
$USER should not expand
NOQUOTE
echo 'single
multi
line' | wc -l
cat << EOF | grep match | wc -l && echo "Found matches" || echo "No matches"
no match
has match
another match
no
EOF
echo $USER
echo "$USER"
echo '$USER'
cat << DATA | grep test | tr 'a-z' 'A-Z' | cat | wc -c
test line one
test line two
other line
DATA
echo start > /tmp/logic_test.txt && cat /tmp/logic_test.txt | wc -c || echo fail
rm -f /tmp/comp_out /tmp/comp1.txt /tmp/logic_test.txt
unset TEST_VAR
unset NUM
exit
