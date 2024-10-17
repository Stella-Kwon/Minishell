#!/bin/bash

# Minishell 테스트 케이스
minishell_output="minishell_test_output.txt"

# Clear previous output
> "$minishell_output"

run_minishell_tests() {
echo "Testing: echo hi" >> "$minishell_output"
echo hi >> "$minishell_output"

echo "Testing: echo hi > 7.txt" >> "$minishell_output"
echo hi > 7.txt
cat 7.txt >> "$minishell_output"

echo "Testing: <1.txt cat" >> "$minishell_output"
<1.txt cat >> "$minishell_output"

echo "Testing: << ll" >> "$minishell_output"
echo "cat << ll" >> "$minishell_output"

echo "Testing: <1.txt cat | sort > 3.txt" >> "$minishell_output"
<1.txt cat | sort > 3.txt
cat 3.txt >> "$minishell_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc -l > 3" >> "$minishell_output"
<1.txt /bin/cat | sort | uniq | wc -l > 3
cat 3 >> "$minishell_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc > 13.txt" >> "$minishell_output"
<1.txt /bin/cat | sort | uniq | wc > 13.txt
cat 13.txt >> "$minishell_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc -l >> 13.txt" >> "$minishell_output"
<1.txt /bin/cat | sort | uniq | wc -l >> 13.txt
tail -n 1 13.txt >> "$minishell_output"

echo "Testing: cat 1.txt | sort | uniq > 8.txt" >> "$minishell_output"
cat 1.txt | sort | uniq > 8.txt
cat 8.txt >> "$minishell_output"

echo "Testing: <1.txt cat | grep pat123 | sort | uniq > 4.txt" >> "$minishell_output"
<1.txt cat | grep pat123 | sort | uniq > 4.txt
cat 4.txt >> "$minishell_output"

echo "Testing: /bin/ls" >> "$minishell_output"
/bin/ls >> "$minishell_output"

echo "Testing: echo hi | echo by" >> "$minishell_output"
echo hi | echo by >> "$minishell_output"

echo "Testing: exit 20 | exit 30" >> "$minishell_output"
(exit 20 | exit 30)
actual=$?
echo "$actual" >> "$minishell_output"

echo "Testing: echo 'cat 1.txt | cat > 3.txt'" >> "$minishell_output"
echo 'cat 1.txt | cat > 3.txt' >> "$minishell_output"

echo "Testing: <1.txt >44.txt" >> "$minishell_output"
<1.txt >44.txt
cat 44.txt >> "$minishell_output"

echo "Testing: cd ." >> "$minishell_output"
cd .
pwd >> "$minishell_output"

echo "Testing: unset PATH" >> "$minishell_output"
unset PATH
env | grep PATH >> "$minishell_output"

echo "Testing: unset HOME" >> "$minishell_output"
unset HOME
env | grep HOME >> "$minishell_output"

echo "Testing: env" >> "$minishell_output"
env >> "$minishell_output"

echo "Testing: pwd" >> "$minishell_output"
pwd >> "$minishell_output"

echo "Testing: \$?" >> "$minishell_output"
echo "$?" >> "$minishell_output"

echo "Testing: <’’" >> "$minishell_output"
echo "This test is expected to produce a syntax error." >> "$minishell_output"

echo "Testing: <1.txt echo hi > 77.txt" >> "$minishell_output"
<1.txt echo hi > 77.txt
cat 77.txt >> "$minishell_output"

echo "Testing: echo 'cat 1.txt | cat > 3.txt'" >> "$minishell_output"
echo 'cat 1.txt | cat > 3.txt' >> "$minishell_output"

echo "Testing: <1.txt >44.txt" >> "$minishell_output"
<1.txt >44.txt
cat 44.txt >> "$minishell_output"

echo "Testing: cd ." >> "$minishell_output"
cd .
pwd >> "$minishell_output"

echo "Testing: unset PATH" >> "$minishell_output"
unset PATH
env | grep PATH >> "$minishell_output"

echo "Testing: unset HOME" >> "$minishell_output"
unset HOME
env | grep HOME >> "$minishell_output"

echo "Testing: env" >> "$minishell_output"
env >> "$minishell_output"

echo "Testing: pwd" >> "$minishell_output"
pwd >> "$minishell_output"

echo "Testing: $?" >> "$minishell_output"
echo "$?" >> "$minishell_output"

echo "Testing: <1.txt echo hi > 77.txt" >> "$minishell_output"
<1.txt echo hi > 77.txt
cat 77.txt >> "$minishell_output"

echo "Test Case: <<<string <out cat" >> "$minishell_output"
{ <<<string <out cat; } >> "$minishell_output"

echo "Test Case: cat > out && grep \"e\"" >> "$minishell_output"
{ echo "example" > out && grep "e" out; } >> "$minishell_output"

echo "Test Case: ls || pwd" >> "$minishell_output"
{ ls || pwd; } >> "$minishell_output"

echo "Test Case: echo string >> out | cat" >> "$minishell_output"
{ echo string >> out | cat out; } >> "$minishell_output"

echo "Test Case: echo -n hello" >> "$minishell_output"
{ echo -n hello; } >> "$minishell_output"

echo "Test Case: export HELLO=123" >> "$minishell_output"
export HELLO=123
{ echo $HELLO; } >> "$minishell_output"

echo "Test Case: export HELLO=\"beautiful world\"" >> "$minishell_output"
export HELLO="beautiful world"
{ echo $HELLO; } >> "$minishell_output"

echo "Test Case: cd obj" >> "$minishell_output"
cd obj
{ pwd; } >> "$minishell_output"

echo "Test Case: echo \$PWD \$OLDPWD" >> "$minishell_output"
{ echo $PWD $OLDPWD; } >> "$minishell_output"

echo "Test Case: doesntexist" >> "$minishell_output"
{ doesntexist 2>&1; } >> "$minishell_output"

echo "Test Case: echo \$?" >> "$minishell_output"
{ echo $?; } >> "$minishell_output"

echo "Test Case: \$EMPTY" >> "$minishell_output"
{ echo $EMPTY; } >> "$minishell_output"

echo "Test Case: echo \$?" >> "$minishell_output"
{ echo $?; } >> "$minishell_output"

echo "Test Case: ps -a" >> "$minishell_output"
{ ps -a; } >> "$minishell_output"

echo "Test Case: echo - \"\" \" \" hello" >> "$minishell_output"
{ echo - "" " " hello; } >> "$minishell_output"

echo "Test Case: unset USER" >> "$minishell_output"
unset USER
env | grep USER >> "$minishell_output"

echo "Test Case: unset PATH" >> "$minishell_output"
unset PATH
env | grep PATH >> "$minishell_output"

echo "Test Case: /bin/ls" >> "$minishell_output"
{ /bin/ls; } >> "$minishell_output"

echo "Test Case: cd .. hi" >> "$minishell_output"
cd .. hi 2>/dev/null
{ pwd; } >> "$minishell_output"

echo "Test Case: cd" >> "$minishell_output"
cd
{ pwd; } >> "$minishell_output"

echo "Test Case: cat <<HEREDOC" >> "$minishell_output"
{
    cat <<HEREDOC
    Hello, World!
HEREDOC
} >> "$minishell_output"

echo "Test Case: cat <<HERE <<DOC" >> "$minishell_output"
{
    cat <<HERE
First Document
HERE
} >> "$minishell_output"

{
    cat <<DOC
Second Document
DOC
} >> "$minishell_output"

echo "Test Case: cat <<HERE | ls" >> "$minishell_output"
{
    cat <<HERE
    This will be piped to ls.
HERE
} | ls >> "$minishell_output"

echo "Test Case: <<<string << here < out cat | grep \"word\" | wc -l" >> "$minishell_output"
{
    <<<string
    cat < out
} | grep "word" | wc -l >> "$minishell_output"

set +e

echo "Test Case: echo hi |    |  < >>" >> "$minishell_output"
minishell -c 'echo hi |    |  < >>' >> "$minishell_output" 2>&1

echo "Test Case: echo hi | < >> | |" >> "$minishell_output"
minishell -c 'echo hi | < >> | |' >> "$minishell_output" 2>&1

echo "Test Case: <<<<" >> "$minishell_output"
minishell -c '<<<<' >> "$minishell_output" 2>&1

echo "Test Case: <<<" >> "$minishell_output"
minishell -c '<<<' >> "$minishell_output" 2>&1

set -e

# set + e

# echo "Test Case: echo hi |    |  < >>" >> "$minishell_output"
# { echo hi |    |  < >>; } >> "$minishell_output"

# echo "Test Case: echo hi | < >> | |" >> "$minishell_output"
# { echo hi | < >> | |;}  >> "$minishell_output"

# echo "Test Case: <<<<" >> "$minishell_output"
# { <<<<;} >> "$minishell_output"

# echo "Test Case: <<<" >> "$minishell_output"
# { <<<;} >> "$minishell_output"

# set - e

echo "All bash tests completed." >> "$minishell_output"
}