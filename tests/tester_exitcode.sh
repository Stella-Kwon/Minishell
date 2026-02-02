#!/usr/bin/env bash

set -u

source "$(dirname "$0")/tester_lib.sh"

TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-5}"
TMP_DIR="$(mktemp -d "${TMPDIR:-/tmp}/minishell-exitcode.XXXXXX")"
trap 'rm -rf "$TMP_DIR"' EXIT

REPORT="$TEST_DIR/result_exitcode.ms"
: > "$REPORT"

echo "# minishell vs bash (per-command exitcodes)" >> "$REPORT"
echo "" >> "$REPORT"
echo "- minishell: \`$MINISHELL_BIN\`" >> "$REPORT"
echo "- bash: \`$BASH_BIN\` (noprofile/norc)" >> "$REPORT"
echo "- cases: \`$CASES_DIR\`" >> "$REPORT"
echo "- timeout: ${TIMEOUT_SECONDS}s" >> "$REPORT"
echo "" >> "$REPORT"

trim() {
  local s="$1"
  s="${s#"${s%%[![:space:]]*}"}"
  s="${s%"${s##*[![:space:]]}"}"
  printf '%s' "$s"
}

extract_heredoc_delim() {
  local line="$1"
  local after
  after="$(printf '%s\n' "$line" | sed -n 's/.*<<[[:space:]]*//p')"
  [ -z "$after" ] && return 1
  printf '%s\n' "$after" | awk '{print $1}'
}

parse_units() {
  local file="$1"
  local out_list="$2"
  : > "$out_list"

  local line t delim in_hd unit in_quote quote_char
  in_hd=0
  in_quote=0
  quote_char=""
  delim=""
  unit=""

  while IFS= read -r line || [ -n "$line" ]; do
    t="$(trim "$line")"

    if [ "$in_hd" -eq 1 ]; then
      unit="${unit}"$'\n'"$line"
      if [ "$t" = "$delim" ]; then
        printf '%s\0' "$unit" >> "$out_list"
        unit=""
        in_hd=0
        delim=""
      fi
      continue
    fi

    if [ "$in_quote" -eq 1 ]; then
      unit="${unit}"$'\n'"$line"
      # Check if line contains the closing quote
      if printf '%s\n' "$line" | grep -qF "$quote_char"; then
        # Count quotes in the line to see if it's closed
        local quote_count=$(printf '%s\n' "$line" | tr -cd "$quote_char" | wc -c)
        if [ $((quote_count % 2)) -eq 1 ]; then
          # Odd number means quote is closed
          printf '%s\0' "$unit" >> "$out_list"
          unit=""
          in_quote=0
          quote_char=""
        fi
      fi
      continue
    fi

    [ -z "$t" ] && continue
    case "$t" in
      \#*) continue ;;
      exit) break ;;
    esac

    delim="$(extract_heredoc_delim "$line")"
    if [ -n "$delim" ]; then
      in_hd=1
      unit="$line"
      continue
    fi

    # Check for unclosed quotes (single or double)
    # Simple approach: count quotes (ignoring escaped ones for now)
    # This is not perfect but should work for most test cases
    local single_count=$(printf '%s\n' "$line" | awk -v RS="'" 'END {print NR-1}')
    local double_count=$(printf '%s\n' "$line" | awk -v RS='"' 'END {print NR-1}')
    
    # If odd number of quotes, might be multiline
    # But we need to check which quote type is open
    if [ $((single_count % 2)) -eq 1 ] && [ $((double_count % 2)) -eq 0 ]; then
      # Single quote started but not closed
      in_quote=1
      quote_char="'"
      unit="$line"
      continue
    elif [ $((double_count % 2)) -eq 1 ] && [ $((single_count % 2)) -eq 0 ]; then
      # Double quote started but not closed
      in_quote=1
      quote_char='"'
      unit="$line"
      continue
    fi

    printf '%s\0' "$line" >> "$out_list"
  done < "$file"
}

printf "%-22s %-6s\n" "case" "res"
pass=0; fail=0

for case_file in $(list_cases); do
  base="$(basename "$case_file")"
  name="${base%.*}"

  unit_list="$TMP_DIR/${name}.units"
  parse_units "$case_file" "$unit_list"

  mini_ec="$TMP_DIR/${name}.mini.ec"
  bash_ec="$TMP_DIR/${name}.bash.ec"
  : > "$mini_ec"; : > "$bash_ec"

  idx=1
  while IFS= read -r -d '' unit_line || [ -n "$unit_line" ]; do
    unit_file="$TMP_DIR/${name}.unit.${idx}.ms"
    printf '%s\n' "$unit_line" > "$unit_file"
    printf 'exit\n' >> "$unit_file"

    run_with_timeout "$TIMEOUT_SECONDS" "$MINISHELL_BIN" < "$unit_file" > /dev/null 2>/dev/null
    mini_status=$?
    run_with_timeout "$TIMEOUT_SECONDS" "$BASH_BIN" --noprofile --norc < "$unit_file" > /dev/null 2>/dev/null
    bash_status=$?

    printf '%d\n' "$mini_status" >> "$mini_ec"
    printf '%d\n' "$bash_status" >> "$bash_ec"
    idx=$((idx + 1))
  done < "$unit_list"

  diff -q "$bash_ec" "$mini_ec" >/dev/null 2>&1
  if [ $? -eq 0 ]; then
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
    echo ""
    echo "[case input]"
    nl -ba "$case_file"
    echo ""
    echo "[exitcodes]"
    line_no=1
    paste "$bash_ec" "$mini_ec" | while IFS=$'\t' read -r b m; do
      printf '#%d bash: %s   mini: %s\n' "$line_no" "$b" "$m"
      line_no=$((line_no + 1))
    done
    echo ""
    if [ "$res" = "FAIL" ]; then
      echo "[diff exitcodes - compact]"
      compact_diff "$bash_ec" "$mini_ec"
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

