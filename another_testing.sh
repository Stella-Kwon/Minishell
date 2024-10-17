#!/bin/bash

GREEN="\033[0;32m"  # 초록색
RED="\033[0;31m"    # 빨간색
NC="\033[0m"        # 기본색으로 리셋

bash_output="bash_output.txt"
minishell_output="minishell_output.txt"

rm -f "$bash_output" "$minishell_output"

exec 2>/dev/null

source ./bash_test.sh
source ./minishell_test.sh

run_bash_tests &
./minishell && run_minishell_tests &


wait


echo "Comparing results..."
# diff "$bash_output" "$minishell_output" > diff_output.txt
diff "$bash_output" "$minishell_output" | sed 's/^.*: //' > diff_output.txt

if [ $? -eq 0 ]; then
    echo -e "${GREEN}ALL_PASSED${NC}"
else
    echo -e "${RED}FAIL!!!${NC}"
    echo -e "${GREEN}Go and see : diff_output.txt.${NC}"

    echo -e "${RED}Diff output:${NC}"
    cat diff_output.txt

fi

rm -f "3" "3.txt" "4.txt" 5.txt 7.txt 8.txt 13.txt 44.txt echo out args[0] command- "$bash_output" "$minishell_output" "diff_output.txt" 