#!/usr/bin/env bash

set -u

source "$(dirname "$0")/tester_lib.sh"

TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-5}"
TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/minishell-fd.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT

REPORT="$TEST_DIR/result_fd.ms"
: > "$REPORT"

echo "# minishell fd/hang tester" >> "$REPORT"
echo "" >> "$REPORT"
echo "- minishell: \`$MINISHELL_BIN\`" >> "$REPORT"
echo "- cases: \`$CASES_DIR\`" >> "$REPORT"
echo "- timeout: ${TIMEOUT_SECONDS}s" >> "$REPORT"
echo "" >> "$REPORT"

wait_with_timeout() {
  local seconds="$1" pid="$2"
  perl -e 'alarm $ARGV[0]; while (kill 0, $ARGV[1]) { select(undef,undef,undef,0.1); }' "$seconds" "$pid"
  local status=$?
  if [ "$status" -eq 142 ] || [ "$status" -eq 124 ]; then
    return 124
  fi
  return 0
}

printf "%-22s %-6s\n" "case" "res"
pass=0; fail=0

for case_file in $(list_cases); do
  base="$(basename "$case_file")"
  name="${base%.*}"

  mini_out="$TMP_DIR/${name}.out"
  mini_err="$TMP_DIR/${name}.err"
  lsof_file="$TMP_DIR/${name}.lsof"

  "$MINISHELL_BIN" < "$case_file" > "$mini_out" 2> "$mini_err" &
  pid=$!

  wait_with_timeout "$TIMEOUT_SECONDS" "$pid"
  status=$?

  if [ "$status" -eq 124 ]; then
    res="FAIL (HANG)"
    fail=$((fail + 1))
    lsof -p "$pid" > "$lsof_file" 2>/dev/null || true
    kill -TERM "$pid" 2>/dev/null || true
    sleep 0.2
    kill -KILL "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
  else
    res="PASS"
    pass=$((pass + 1))
    wait "$pid" 2>/dev/null || true
  fi

  printf "%-22s %-6s\n" "$name" "$res"

  {
    echo "=== CASE: $name ==="
    echo "RESULT: $res"
    echo ""
    echo "[case input]"
    nl -ba "$case_file"
    echo ""
    if [ "$res" != "PASS" ]; then
      echo "[lsof snapshot]"
      if [ -s "$lsof_file" ]; then
        sed -n '1,200p' "$lsof_file"
      else
        echo "(empty)"
      fi
      echo ""
      echo "[note] Hangs often happen when a pipe write-end is left open, so EOF never arrives."
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

