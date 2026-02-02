echo a   b   c
echo ""
echo ''
echo '""'
echo "''"
echo "$ # @ ! & | > <"
echo '$ # @ ! & | > <'
echo "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
echo a b c d e f g h i j k l m n o p q r s t u v w x y z
cat < /nonexistent/file
echo test > /nonexistent/dir/file
echo first > /tmp/edge_test.txt
echo second > /tmp/edge_test.txt
cat /tmp/edge_test.txt
echo "" | cat | wc -c
echo a    |    cat    |    wc -c
echo b   &&   echo c
echo d   ||   echo e
echo "line1
line2"
echo $NONEXISTENT_VAR
echo "$NONEXISTENT_VAR"
rm -f /tmp/edge_test.txt

exit
