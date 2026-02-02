cat <<< "herestring test"
cat <<< "hello world" | wc -c
grep hello <<< "hello world"
wc -w <<< "one two three"
cat <<< "$USER"
cat <<< 'no expand: $USER'
cat <<< "multi word string" | tr ' ' '\n' | wc -l
echo test | cat <<< "herestring in pipe"
cat <<< hello
cat <<< "exit code: $?"
/bin/false
cat <<< "after false: $?"
cat <<< "line 1" && cat <<< "line 2"
cat <<< "before or" || echo "should not print"
cat <<< first <<< second <<< third
cat <<< "hello" <<< "world"
cat <<< hello < /etc/hostname
exit


