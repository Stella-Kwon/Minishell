cat << EOF
line 1
line 2
line 3
EOF
cat << EOF | wc -l
a
b
c
EOF
cat << EOF | grep pattern
no match
pattern here
another line
EOF
cat << EOF
Current user: $USER
Exit code: $?
EOF
cat << 'EOF'
This should not expand: $USER
EOF
cat << "END"
Also no expansion: $USER
END
cat << EOF1 | cat << EOF2 | cat
data from first
EOF1
data from second
EOF2
cat << DATA | grep success | wc -l
failure
success
another success
DATA
cat << EOF | cat && echo ok1 || echo fail1
test
EOF
cat << EOF | grep missing || echo ok2
test
EOF
cat << EOF | cat | wc -l && echo ok3 || echo no3
line
line
EOF
cat << EOF | wc -l && /bin/false || echo ok4
one
two
three
EOF
cat << EOF | cat && /bin/false || /bin/true && echo ok5
$USER
EOF
cat << EOF | cat | cat && echo ok6 || echo no6
line
EOF
cat << hi <<< hello <<< why
ignored by herestring
hi
cat <<< hi << hello
heredoc wins
hello
cat << EOF <<< "after"
heredoc ignored
EOF
exit
