true && echo "ok1"
false && echo "not shown"
false || echo "ok2"
true || echo "not shown"
true && true && echo "ok3"
true && false && echo "not shown"
false || false || echo "ok4"
false || true || echo "not shown"
false || true && echo "ok5"
true && false || echo "ok6"
false || true && echo ok7 || echo no7
true && false || true && echo ok8
echo hi | wc -c && echo "ok9"
echo hi | grep missing || echo "ok10"
echo hi | wc -c && false || echo "ok11"
/bin/false || /bin/true && /bin/false || echo ok12
echo test | wc -c && /bin/false || echo ok13
echo a | grep a && echo "ok14"
echo a | grep z || echo "ok15"
true && true && true && echo "ok16"
false || false || false || echo "ok17"
exit
