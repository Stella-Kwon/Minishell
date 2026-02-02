# minishell vs bash (per-command exitcodes)

- minishell: `/Users/suminkwon/Minishell/minishell`
- bash: `/bin/bash` (noprofile/norc)
- cases: `/Users/suminkwon/Minishell/tests/cases`
- timeout: 5s

=== CASE: 01_basic ===
RESULT: PASS

[case input]
     1	echo hello
     2	echo hello world
     3	echo
     4	pwd
     5	env | grep USER | head -n 1
     6	echo first
     7	echo second
     8	echo third
     9	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0

----------------------------------------

=== CASE: 02_builtins ===
RESULT: PASS

[case input]
     1	echo hello world
     2	echo "hello world"
     3	echo 'hello world'
     4	pwd
     5	export HELLO=123
     6	echo $HELLO
     7	export WORLD=456
     8	echo $HELLO $WORLD
     9	unset HELLO
    10	echo $HELLO
    11	echo $WORLD
    12	pwd
    13	cd
    14	cd /usr
    15	pwd
    16	cd -
    17	pwd
    18	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0
#16 bash: 1   mini: 1
#17 bash: 0   mini: 0

----------------------------------------

=== CASE: 03_quotes ===
RESULT: PASS

[case input]
     1	echo 'hello world'
     2	echo '$USER'
     3	echo 'pipe | inside'
     4	echo "hello world"
     5	echo "$USER"
     6	echo "Current user: $USER"
     7	echo "pipe | inside quotes"
     8	echo "redirect > inside"
     9	echo 'redirect > inside'
    10	echo 'line 1
    11	line 2
    12	line 3'
    13	echo "line 1
    14	line 2
    15	line 3"
    16	echo "Hello $USER
    17	on multiple
    18	lines"
    19	echo ""
    20	echo ''
    21	echo "double" 'single' "double again"
    22	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0

----------------------------------------

=== CASE: 04_redirections ===
RESULT: PASS

[case input]
     1	echo hello > /tmp/minishell_test_out
     2	cat /tmp/minishell_test_out
     3	echo world > /tmp/minishell_test_out
     4	cat /tmp/minishell_test_out
     5	echo first > /tmp/minishell_test_append
     6	echo second >> /tmp/minishell_test_append
     7	echo third >> /tmp/minishell_test_append
     8	cat /tmp/minishell_test_append
     9	echo "input data" > /tmp/minishell_test_in
    10	cat < /tmp/minishell_test_in
    11	cat < /tmp/minishell_test_in > /tmp/minishell_test_out2
    12	cat /tmp/minishell_test_out2
    13	echo test > /tmp/test1.txt
    14	echo test2 > /tmp/test2.txt
    15	cat < /tmp/test1.txt > /tmp/test3.txt
    16	cat /tmp/test3.txt
    17	cat < /tmp/minishell_test_in | wc -c
    18	cat < /tmp/minishell_missing_file
    19	rm -f /tmp/minishell_test_out /tmp/minishell_test_in /tmp/minishell_test_out2 /tmp/minishell_test_append
    20	rm -f /tmp/test1.txt /tmp/test2.txt /tmp/test3.txt
    21	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0
#16 bash: 0   mini: 0
#17 bash: 0   mini: 0
#18 bash: 1   mini: 1
#19 bash: 0   mini: 0
#20 bash: 0   mini: 0

----------------------------------------

=== CASE: 05_pipes ===
RESULT: PASS

[case input]
     1	echo hello | cat
     2	echo hello | wc -c
     3	echo hi | cat | wc -c
     4	echo test | cat | cat | wc -c
     5	echo "hello world" | grep world
     6	echo "hello world" | grep missing
     7	echo "HELLO" | tr 'A-Z' 'a-z'
     8	echo "x y z" | tr -d ' '
     9	echo "c
    10	b
    11	a" | sort
    12	echo "one two three" | wc -w
    13	echo "line1
    14	line2
    15	line3" | wc -l
    16	echo test | cat | cat | cat | wc -c
    17	echo data | cat | cat | cat | cat | cat | cat | cat | cat | cat | wc -c
    18	cat << EOF | grep x | wc -l
    19	x
    20	y
    21	x
    22	z
    23	EOF
    24	echo "hello world" | tr ' ' '\n' | wc -l
    25	echo "a b c d e" | tr ' ' '\n' | sort | head -n 2
    26	echo "" | cat | wc -c
    27	cat << EOF | wc -l
    28	a
    29	b
    30	c
    31	EOF
    32	echo hi | cat | cat | wc -c
    33	cat << EOF | grep x | wc -l
    34	x
    35	y
    36	EOF
    37	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 1   mini: 1
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0
#16 bash: 0   mini: 0
#17 bash: 0   mini: 0
#18 bash: 0   mini: 0
#19 bash: 0   mini: 0
#20 bash: 0   mini: 0

----------------------------------------

=== CASE: 06_heredoc ===
RESULT: PASS

[case input]
     1	cat << EOF
     2	line 1
     3	line 2
     4	line 3
     5	EOF
     6	cat << EOF | wc -l
     7	a
     8	b
     9	c
    10	EOF
    11	cat << EOF | grep pattern
    12	no match
    13	pattern here
    14	another line
    15	EOF
    16	cat << EOF
    17	Current user: $USER
    18	Exit code: $?
    19	EOF
    20	cat << 'EOF'
    21	This should not expand: $USER
    22	EOF
    23	cat << "END"
    24	Also no expansion: $USER
    25	END
    26	cat << EOF1 | cat << EOF2 | cat
    27	data from first
    28	EOF1
    29	data from second
    30	EOF2
    31	cat << DATA | grep success | wc -l
    32	failure
    33	success
    34	another success
    35	DATA
    36	cat << EOF | cat && echo ok1 || echo fail1
    37	test
    38	EOF
    39	cat << EOF | grep missing || echo ok2
    40	test
    41	EOF
    42	cat << EOF | cat | wc -l && echo ok3 || echo no3
    43	line
    44	line
    45	EOF
    46	cat << EOF | wc -l && /bin/false || echo ok4
    47	one
    48	two
    49	three
    50	EOF
    51	cat << EOF | cat && /bin/false || /bin/true && echo ok5
    52	$USER
    53	EOF
    54	cat << EOF | cat | cat && echo ok6 || echo no6
    55	line
    56	EOF
    57	cat << hi <<< hello <<< why
    58	ignored by herestring
    59	hi
    60	cat <<< hi << hello
    61	heredoc wins
    62	hello
    63	cat << EOF <<< "after"
    64	heredoc ignored
    65	EOF
    66	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0

----------------------------------------

=== CASE: 07_herestring ===
RESULT: PASS

[case input]
     1	cat <<< "herestring test"
     2	cat <<< "hello world" | wc -c
     3	grep hello <<< "hello world"
     4	wc -w <<< "one two three"
     5	cat <<< "$USER"
     6	cat <<< 'no expand: $USER'
     7	cat <<< "multi word string" | tr ' ' '\n' | wc -l
     8	echo test | cat <<< "herestring in pipe"
     9	cat <<< hello
    10	cat <<< "exit code: $?"
    11	/bin/false
    12	cat <<< "after false: $?"
    13	cat <<< "line 1" && cat <<< "line 2"
    14	cat <<< "before or" || echo "should not print"
    15	cat <<< first <<< second <<< third
    16	cat <<< "hello" <<< "world"
    17	cat <<< hello < /etc/hostname
    18	exit
    19	

[exitcodes]

----------------------------------------

=== CASE: 08_logic_ops ===
RESULT: PASS

[case input]
     1	true && echo "ok1"
     2	false && echo "not shown"
     3	false || echo "ok2"
     4	true || echo "not shown"
     5	true && true && echo "ok3"
     6	true && false && echo "not shown"
     7	false || false || echo "ok4"
     8	false || true || echo "not shown"
     9	false || true && echo "ok5"
    10	true && false || echo "ok6"
    11	false || true && echo ok7 || echo no7
    12	true && false || true && echo ok8
    13	echo hi | wc -c && echo "ok9"
    14	echo hi | grep missing || echo "ok10"
    15	echo hi | wc -c && false || echo "ok11"
    16	/bin/false || /bin/true && /bin/false || echo ok12
    17	echo test | wc -c && /bin/false || echo ok13
    18	echo a | grep a && echo "ok14"
    19	echo a | grep z || echo "ok15"
    20	true && true && true && echo "ok16"
    21	false || false || false || echo "ok17"
    22	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 1   mini: 1
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 1   mini: 1
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0
#16 bash: 0   mini: 0
#17 bash: 0   mini: 0
#18 bash: 0   mini: 0
#19 bash: 0   mini: 0
#20 bash: 0   mini: 0
#21 bash: 0   mini: 0

----------------------------------------

=== CASE: 09_exit_codes ===
RESULT: PASS

[case input]
     1	./no_such_command
     2	echo "data" > /tmp/minishell_no_exec
     3	/bin/chmod 644 /tmp/minishell_no_exec
     4	/tmp/minishell_no_exec
     5	/bin/mkdir -p /tmp/minishell_exec_dir
     6	/bin/chmod 755 /tmp/minishell_exec_dir
     7	/tmp/minishell_exec_dir
     8	echo test
     9	/bin/rm -f /tmp/minishell_no_exec
    10	/bin/rm -rf /tmp/minishell_exec_dir
    11	exit

[exitcodes]
#1 bash: 127   mini: 127
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 126   mini: 126
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 126   mini: 126
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0

----------------------------------------

=== CASE: 10_edge_cases ===
RESULT: PASS

[case input]
     1	echo a   b   c
     2	echo ""
     3	echo ''
     4	echo '""'
     5	echo "''"
     6	echo "$ # @ ! & | > <"
     7	echo '$ # @ ! & | > <'
     8	echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
     9	echo a b c d e f g h i j k l m n o p q r s t u v w x y z
    10	cat < /nonexistent/file
    11	echo test > /nonexistent/dir/file
    12	echo first > /tmp/edge_test.txt
    13	echo second > /tmp/edge_test.txt
    14	cat /tmp/edge_test.txt
    15	echo "" | cat | wc -c
    16	echo a    |    cat    |    wc -c
    17	echo b   &&   echo c
    18	echo d   ||   echo e
    19	echo "line1
    20	line2"
    21	echo $NONEXISTENT_VAR
    22	echo "$NONEXISTENT_VAR"
    23	rm -f /tmp/edge_test.txt
    24	
    25	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 1   mini: 1
#11 bash: 1   mini: 1
#12 bash: 0   mini: 0
#13 bash: 0   mini: 0
#14 bash: 0   mini: 0
#15 bash: 0   mini: 0
#16 bash: 0   mini: 0
#17 bash: 0   mini: 0
#18 bash: 0   mini: 0
#19 bash: 0   mini: 0
#20 bash: 0   mini: 0
#21 bash: 0   mini: 0
#22 bash: 0   mini: 0

----------------------------------------

=== CASE: 11_comprehensive ===
RESULT: PASS

[case input]
     1	export TEST_VAR=integration_test
     2	export NUM=42
     3	cat << EOF | grep success | wc -l > /tmp/comp_out && cat /tmp/comp_out || echo fail
     4	failure line
     5	success line 1
     6	another failure
     7	success line 2
     8	EOF
     9	echo "User: $USER
    10	Test: $TEST_VAR
    11	Number: $NUM" | grep Test | wc -l
    12	true && echo "step1" || echo "skip" && echo "step2"
    13	false || echo "recover1" && echo "continue1" || echo "skip"
    14	cat << DATA | grep "$USER" | wc -l | cat
    15	test line
    16	username: $USER
    17	another: $TEST_VAR
    18	DATA
    19	cat << EOF1 | cat << EOF2 | cat
    20	first heredoc
    21	EOF1
    22	second heredoc
    23	EOF2
    24	echo "test data" > /tmp/comp1.txt && cat < /tmp/comp1.txt | wc -c || echo fail
    25	echo "multi
    26	line
    27	echo" | wc -l
    28	cat << 'NOQUOTE' | cat
    29	$USER should not expand
    30	NOQUOTE
    31	echo 'single
    32	multi
    33	line' | wc -l
    34	cat << EOF | grep match | wc -l && echo "Found matches" || echo "No matches"
    35	no match
    36	has match
    37	another match
    38	no
    39	EOF
    40	echo $USER
    41	echo "$USER"
    42	echo '$USER'
    43	cat << DATA | grep test | tr 'a-z' 'A-Z' | cat | wc -c
    44	test line one
    45	test line two
    46	other line
    47	DATA
    48	echo start > /tmp/logic_test.txt && cat /tmp/logic_test.txt | wc -c || echo fail
    49	rm -f /tmp/comp_out /tmp/comp1.txt /tmp/logic_test.txt
    50	unset TEST_VAR
    51	unset NUM
    52	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0

----------------------------------------

=== CASE: fd_management ===
RESULT: PASS

[case input]
     1	echo test | cat
     2	echo data | cat | cat | cat | cat | wc -c
     3	echo "stress test" | cat | cat | cat | cat | cat | cat | cat | cat | cat | wc -c
     4	cat << EOF | cat
     5	heredoc line 1
     6	heredoc line 2
     7	EOF
     8	cat << EOF1 | cat << EOF2 | cat
     9	first
    10	EOF1
    11	second
    12	EOF2
    13	cat << DATA | cat | cat | cat | wc -l
    14	line1
    15	line2
    16	line3
    17	DATA
    18	echo test > /tmp/fd_test.txt
    19	cat < /tmp/fd_test.txt | cat | wc -c
    20	cat << EOF | grep test | cat > /tmp/fd_test2.txt
    21	test line
    22	other
    23	EOF
    24	cat /tmp/fd_test2.txt
    25	cat << D1 | cat << D2 | cat << D3 | cat | cat | wc -l > /tmp/stress.txt && cat /tmp/stress.txt
    26	data1
    27	D1
    28	data2
    29	D2
    30	data3
    31	D3
    32	rm -f /tmp/fd_test.txt /tmp/fd_test2.txt /tmp/stress.txt
    33	exit

[exitcodes]
#1 bash: 0   mini: 0
#2 bash: 0   mini: 0
#3 bash: 0   mini: 0
#4 bash: 0   mini: 0
#5 bash: 0   mini: 0
#6 bash: 0   mini: 0
#7 bash: 0   mini: 0
#8 bash: 0   mini: 0
#9 bash: 0   mini: 0
#10 bash: 0   mini: 0
#11 bash: 0   mini: 0
#12 bash: 0   mini: 0

----------------------------------------


## Summary

- PASS: 12
- FAIL: 0
