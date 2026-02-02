#!/usr/bin/env bash

set -u

source "$(dirname "$0")/tester_lib.sh"

TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-5}"
TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/minishell-cases.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT

REPORT="$TEST_DIR/result_cases.ms"
: > "$REPORT"

echo "# minishell vs bash (stdout/stderr)" >> "$REPORT"
echo "" >> "$REPORT"
echo "- minishell: \`$MINISHELL_BIN\`" >> "$REPORT"
echo "- bash: \`$BASH_BIN\` (noprofile/norc)" >> "$REPORT"
echo "- cases: \`$CASES_DIR\`" >> "$REPORT"
echo "- timeout: ${TIMEOUT_SECONDS}s" >> "$REPORT"
echo "" >> "$REPORT"

printf "%-22s %-6s\n" "case" "res"
pass=0; fail=0

for case_file in $(list_cases); do
  base="$(basename "$case_file")"
  name="${base%.*}"

  mini_out="$TMP_DIR/${name}.mini.out"
  mini_err="$TMP_DIR/${name}.mini.err"
  bash_out="$TMP_DIR/${name}.bash.out"
  bash_err="$TMP_DIR/${name}.bash.err"

  run_with_timeout "$TIMEOUT_SECONDS" "$MINISHELL_BIN" < "$case_file" > "$mini_out" 2> "$mini_err"
  mini_status=$?
  run_with_timeout "$TIMEOUT_SECONDS" "$BASH_BIN" --noprofile --norc < "$case_file" > "$bash_out" 2> "$bash_err"
  bash_status=$?

  normalize_minishell_output < "$mini_out" > "$mini_out.norm"
  normalize_minishell_error  < "$mini_err" > "$mini_err.norm"
  cat "$bash_out" > "$bash_out.norm"
  normalize_bash_error < "$bash_err" > "$bash_err.norm"

  out_bad=0
  err_bad=0
  diff -q "$bash_out.norm" "$mini_out.norm" >/dev/null 2>&1 || out_bad=1
  diff -q "$bash_err.norm" "$mini_err.norm" >/dev/null 2>&1 || err_bad=1

  timeout_flag=0
  if [ "$mini_status" -eq 124 ] || [ "$bash_status" -eq 124 ]; then
    timeout_flag=1
  fi

  if [ "$out_bad" -eq 0 ] && [ "$err_bad" -eq 0 ] && [ "$timeout_flag" -eq 0 ]; then
    res="PASS"
    pass=$((pass + 1))
  else
    res="FAIL"
    fail=$((fail + 1))
  fi

  printf "%-22s %-6s\n" "$name" "$res"

  {
    echo "=== CASE: $name ==="
    echo "RESULT: $res"
    if [ "$timeout_flag" -eq 1 ]; then
      echo "TIMEOUT: yes (exit=124)"
    else
      echo "TIMEOUT: no"
    fi
    echo ""
    echo "[case input]"
    nl -ba "$case_file" | sed -e '$a\'
    echo ""
    echo ""

    echo "[minishell stdout]"
    [ -s "$mini_out.norm" ] && cat "$mini_out.norm" || echo "(empty)"
    echo ""
    echo "[minishell stderr]"
    [ -s "$mini_err.norm" ] && cat "$mini_err.norm" || echo "(empty)"
    echo ""
    echo "[bash stdout]"
    [ -s "$bash_out.norm" ] && cat "$bash_out.norm" || echo "(empty)"
    echo ""
    echo "[bash stderr]"
    [ -s "$bash_err.norm" ] && cat "$bash_err.norm" || echo "(empty)"
    echo ""
    if [ "$out_bad" -eq 1 ]; then
      echo "[diff stdout - compact]"
      compact_diff "$bash_out.norm" "$mini_out.norm"
      echo ""
    fi
    if [ "$err_bad" -eq 1 ]; then
      echo "[diff stderr - compact]"
      compact_diff "$bash_err.norm" "$mini_err.norm"
      echo ""
    fi
    echo "----------------------------------------"
    echo ""
  } >> "$REPORT"
done

echo "" >> "$REPORT"
echo "## Summary" >> "$REPORT"
echo "" >> "$REPORT"
echo "- PASS: $pass" >> "$REPORT"
echo "- FAIL: $fail" >> "$REPORT"

echo ""
echo "PASS: $pass  FAIL: $fail"
echo "Report: $REPORT"

[ "$fail" -eq 0 ] || exit 1

