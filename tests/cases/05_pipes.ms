echo hello | cat
echo hello | wc -c
echo hi | cat | wc -c
echo test | cat | cat | wc -c
echo "hello world" | grep world
echo "hello world" | grep missing
echo "HELLO" | tr 'A-Z' 'a-z'
echo "x y z" | tr -d ' '
echo "c
b
a" | sort
echo "one two three" | wc -w
echo "line1
line2
line3" | wc -l
echo test | cat | cat | cat | wc -c
echo data | cat | cat | cat | cat | cat | cat | cat | cat | cat | wc -c
cat << EOF | grep x | wc -l
x
y
x
z
EOF
echo "hello world" | tr ' ' '\n' | wc -l
echo "a b c d e" | tr ' ' '\n' | sort | head -n 2
echo "" | cat | wc -c
cat << EOF | wc -l
a
b
c
EOF
echo hi | cat | cat | wc -c
cat << EOF | grep x | wc -l
x
y
EOF
exit
