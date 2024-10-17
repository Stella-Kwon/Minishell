#!/bin/bash

# Minishell 테스트 케이스
bash_output="bash_output.txt"

# Clear previous output
> "$bash_output"

run_bash_tests() {
echo "Testing: echo hi" >> "$bash_output"
echo hi >> "$bash_output"

echo "Testing: echo hi > 7.txt" >> "$bash_output"
echo hi > 7.txt
cat 7.txt >> "$bash_output"

echo "Testing: <1.txt cat" >> "$bash_output"
<1.txt cat >> "$bash_output"

echo "Testing: << ll" >> "$bash_output"
echo "cat << ll" >> "$bash_output"

echo "Testing: <1.txt cat | sort > 3.txt" >> "$bash_output"
<1.txt cat | sort > 3.txt
cat 3.txt >> "$bash_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc -l > 3" >> "$bash_output"
<1.txt /bin/cat | sort | uniq | wc -l > 3
cat 3 >> "$bash_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc > 13.txt" >> "$bash_output"
<1.txt /bin/cat | sort | uniq | wc > 13.txt
cat 13.txt >> "$bash_output"

echo "Testing: <1.txt /bin/cat | sort | uniq | wc -l >> 13.txt" >> "$bash_output"
<1.txt /bin/cat | sort | uniq | wc -l >> 13.txt
tail -n 1 13.txt >> "$bash_output"

echo "Testing: cat 1.txt | sort | uniq > 8.txt" >> "$bash_output"
cat 1.txt | sort | uniq > 8.txt
cat 8.txt >> "$bash_output"

echo "Testing: <1.txt cat | grep pat123 | sort | uniq > 4.txt" >> "$bash_output"
<1.txt cat | grep pat123 | sort | uniq > 4.txt
cat 4.txt >> "$bash_output"

echo "Testing: /bin/ls" >> "$bash_output"
/bin/ls >> "$bash_output"

echo "Testing: echo hi | echo by" >> "$bash_output"
echo hi | echo by >> "$bash_output"

echo "Testing: exit 20 | exit 30" >> "$bash_output"
(exit 20 | exit 30)
actual=$?
echo "$actual" >> "$bash_output"

echo "Testing: echo 'cat 1.txt | cat > 3.txt'" >> "$bash_output"
echo 'cat 1.txt | cat > 3.txt' >> "$bash_output"

echo "Testing: <1.txt >44.txt" >> "$bash_output"
<1.txt >44.txt
cat 44.txt >> "$bash_output"

echo "Testing: cd ." >> "$bash_output"
cd .
pwd >> "$bash_output"

echo "Testing: unset PATH" >> "$bash_output"
unset PATH
env | grep PATH >> "$bash_output"

echo "Testing: unset HOME" >> "$bash_output"
unset HOME
env | grep HOME >> "$bash_output"

echo "Testing: env" >> "$bash_output"
env >> "$bash_output"

echo "Testing: pwd" >> "$bash_output"
pwd >> "$bash_output"

echo "Testing: \$?" >> "$bash_output"
echo "$?" >> "$bash_output"

echo "Testing: <’’" >> "$bash_output"
echo "This test is expected to produce a syntax error." >> "$bash_output"

echo "Testing: <1.txt echo hi > 77.txt" >> "$bash_output"
<1.txt echo hi > 77.txt
cat 77.txt >> "$bash_output"

echo "Testing: echo 'cat 1.txt | cat > 3.txt'" >> "$bash_output"
echo 'cat 1.txt | cat > 3.txt' >> "$bash_output"

echo "Testing: <1.txt >44.txt" >> "$bash_output"
<1.txt >44.txt
cat 44.txt >> "$bash_output"

echo "Testing: cd ." >> "$bash_output"
cd .
pwd >> "$bash_output"

echo "Testing: unset PATH" >> "$bash_output"
unset PATH
env | grep PATH >> "$bash_output"

echo "Testing: unset HOME" >> "$bash_output"
unset HOME
env | grep HOME >> "$bash_output"

echo "Testing: env" >> "$bash_output"
env >> "$bash_output"

echo "Testing: pwd" >> "$bash_output"
pwd >> "$bash_output"

echo "Testing: $?" >> "$bash_output"
echo "$?" >> "$bash_output"

echo "Testing: <1.txt echo hi > 77.txt" >> "$bash_output"
<1.txt echo hi > 77.txt
cat 77.txt >> "$bash_output"

echo "Test Case: <<<string <out cat" >> "$bash_output"
{ <<<string <out cat; } >> "$bash_output"

echo "Test Case: cat > out && grep \"e\"" >> "$bash_output"
{ echo "example" > out && grep "e" out; } >> "$bash_output"

echo "Test Case: ls || pwd" >> "$bash_output"
{ ls || pwd; } >> "$bash_output"

echo "Test Case: echo string >> out | cat" >> "$bash_output"
{ echo string >> out | cat out; } >> "$bash_output"

echo "Test Case: echo -n hello" >> "$bash_output"
{ echo -n hello; } >> "$bash_output"

echo "Test Case: export HELLO=123" >> "$bash_output"
export HELLO=123
{ echo $HELLO; } >> "$bash_output"

echo "Test Case: export HELLO=\"beautiful world\"" >> "$bash_output"
export HELLO="beautiful world"
{ echo $HELLO; } >> "$bash_output"

echo "Test Case: cd obj" >> "$bash_output"
cd obj
{ pwd; } >> "$bash_output"

echo "Test Case: echo \$PWD \$OLDPWD" >> "$bash_output"
{ echo $PWD $OLDPWD; } >> "$bash_output"

echo "Test Case: doesntexist" >> "$bash_output"
{ doesntexist 2>&1; } >> "$bash_output"

echo "Test Case: echo \$?" >> "$bash_output"
{ echo $?; } >> "$bash_output"

echo "Test Case: \$EMPTY" >> "$bash_output"
{ echo $EMPTY; } >> "$bash_output"

echo "Test Case: echo \$?" >> "$bash_output"
{ echo $?; } >> "$bash_output"

echo "Test Case: ps -a" >> "$bash_output"
{ ps -a; } >> "$bash_output"

echo "Test Case: echo - \"\" \" \" hello" >> "$bash_output"
{ echo - "" " " hello; } >> "$bash_output"

echo "Test Case: unset USER" >> "$bash_output"
unset USER
env | grep USER >> "$bash_output"

echo "Test Case: unset PATH" >> "$bash_output"
unset PATH
env | grep PATH >> "$bash_output"

echo "Test Case: /bin/ls" >> "$bash_output"
{ /bin/ls; } >> "$bash_output"

echo "Test Case: cd .. hi" >> "$bash_output"
cd .. hi 2>/dev/null
{ pwd; } >> "$bash_output"

echo "Test Case: cd" >> "$bash_output"
cd
{ pwd; } >> "$bash_output"

echo "Test Case: cat <<HEREDOC" >> "$bash_output"
{
    cat <<HEREDOC
    Hello, World!
HEREDOC
} >> "$bash_output"


echo "Test Case: cat <<HERE <<DOC" >> "$bash_output"
{
    cat <<HERE
First Document
HERE
} >> "$bash_output"

{
    cat <<DOC
Second Document
DOC
} >> "$bash_output"

echo "Test Case: cat <<HERE | ls" >> "$bash_output"
{
    cat <<HERE
    This will be piped to ls.
HERE
} | ls >> "$bash_output"

echo "Test Case: <<<string << here < out cat | grep \"word\" | wc -l" >> "$bash_output"
{
    <<<string
    cat < out
} | grep "word" | wc -l >> "$bash_output"

echo "All bash tests completed." >> "$bash_output"
}