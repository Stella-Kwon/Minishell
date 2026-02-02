#!/usr/bin/env bash

set -u

source "$(dirname "$0")/tester_lib.sh"

TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-30}"
TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/minishell-valgrind.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT

REPORT="$TEST_DIR/result_valgrind.ms"
: > "$REPORT"

echo "# Valgrind Memory Leak Test" >> "$REPORT"
echo "" >> "$REPORT"
echo "- minishell: \`$MINISHELL_BIN\`" >> "$REPORT"
echo "- valgrind: \`$(which valgrind 2>/dev/null || echo 'not found')\`" >> "$REPORT"
echo "- test case: \`$CASES_DIR/valgrind_test.ms\`" >> "$REPORT"
echo "- timeout: ${TIMEOUT_SECONDS}s" >> "$REPORT"
echo "" >> "$REPORT"

if ! command -v valgrind &> /dev/null; then
    echo "ERROR: valgrind not found. Please install valgrind first." >&2
    echo "ERROR: valgrind not found" >> "$REPORT"
    exit 1
fi

if [ ! -f "$CASES_DIR/valgrind_test.ms" ]; then
    echo "ERROR: Test case not found: $CASES_DIR/valgrind_test.ms" >&2
    echo "ERROR: Test case not found" >> "$REPORT"
    exit 1
fi

echo "Running valgrind on valgrind_test.ms..."
echo "This may take a while..."

valgrind_out="$TMP_DIR/valgrind.out"
valgrind_err="$TMP_DIR/valgrind.err"

# Run valgrind with timeout
run_with_timeout "$TIMEOUT_SECONDS" \
    valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --track-fds=yes \
    --trace-children=yes \
    --error-exitcode=1 \
    --log-file="$valgrind_out" \
    "$MINISHELL_BIN" < "$CASES_DIR/valgrind_test.ms" > "$TMP_DIR/minishell.out" 2> "$TMP_DIR/minishell.err"

valgrind_status=$?

{
    echo "=== Valgrind Summary ==="
    echo ""
    
    if [ "$valgrind_status" -eq 124 ]; then
        echo "RESULT: TIMEOUT (exceeded ${TIMEOUT_SECONDS}s)"
        echo ""
    elif [ "$valgrind_status" -eq 0 ]; then
        echo "RESULT: PASS (no memory errors detected)"
        echo ""
    else
        echo "RESULT: FAIL (memory errors detected, exit code: $valgrind_status)"
        echo ""
    fi
    
    echo "[valgrind log]"
    if [ -f "$valgrind_out" ]; then
        cat "$valgrind_out"
    else
        echo "(valgrind log not found)"
    fi
    echo ""
    echo ""
    
    echo "[minishell stdout]"
    if [ -s "$TMP_DIR/minishell.out" ]; then
        cat "$TMP_DIR/minishell.out"
    else
        echo "(empty)"
    fi
    echo ""
    echo ""
    
    echo "[minishell stderr]"
    if [ -s "$TMP_DIR/minishell.err" ]; then
        cat "$TMP_DIR/minishell.err"
    else
        echo "(empty)"
    fi
    echo ""
    echo "----------------------------------------"
    echo ""
} >> "$REPORT"

# Extract key information from valgrind output
if [ -f "$valgrind_out" ]; then
    definitely_lost=$(grep -c "definitely lost" "$valgrind_out" 2>/dev/null || echo "0")
    indirectly_lost=$(grep -c "indirectly lost" "$valgrind_out" 2>/dev/null || echo "0")
    possibly_lost=$(grep -c "possibly lost" "$valgrind_out" 2>/dev/null || echo "0")
    still_reachable=$(grep -c "still reachable" "$valgrind_out" 2>/dev/null || echo "0")
    error_summary=$(grep "ERROR SUMMARY" "$valgrind_out" 2>/dev/null | tail -1 || echo "")
    
    {
        echo "## Memory Leak Summary" >> "$REPORT"
        echo "" >> "$REPORT"
        echo "- Definitely lost: $definitely_lost" >> "$REPORT"
        echo "- Indirectly lost: $indirectly_lost" >> "$REPORT"
        echo "- Possibly lost: $possibly_lost" >> "$REPORT"
        echo "- Still reachable: $still_reachable" >> "$REPORT"
        if [ -n "$error_summary" ]; then
            echo "- $error_summary" >> "$REPORT"
        fi
        echo "" >> "$REPORT"
    } >> "$REPORT"
    
    echo ""
    echo "Memory Leak Summary:"
    echo "  Definitely lost: $definitely_lost"
    echo "  Indirectly lost: $indirectly_lost"
    echo "  Possibly lost: $possibly_lost"
    echo "  Still reachable: $still_reachable"
    if [ -n "$error_summary" ]; then
        echo "  $error_summary"
    fi
    echo ""
fi

echo "Report: $REPORT"
echo ""

# Check if there are actual memory leaks (definitely or indirectly lost)
if [ "$definitely_lost" -gt 0 ] || [ "$indirectly_lost" -gt 0 ]; then
    echo "❌ FAIL: Memory leaks detected!"
    exit 1
elif [ "$valgrind_status" -eq 124 ]; then
    echo "⚠️  TIMEOUT: Test exceeded time limit"
    exit 1
elif [ "$valgrind_status" -ne 0 ]; then
    echo "❌ FAIL: Valgrind detected errors"
    exit 1
else
    echo "✅ PASS: No memory leaks detected"
    exit 0
fi


