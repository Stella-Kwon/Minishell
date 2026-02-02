#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TEST_DIR="$ROOT_DIR/tests"
CASES_DIR="$TEST_DIR/cases"
MINISHELL_BIN="${MINISHELL_BIN:-$ROOT_DIR/minishell}"
BASH_BIN="${BASH_BIN:-/bin/bash}"

list_cases() {
  local f
  for f in "$CASES_DIR"/*.ms; do
    [ -e "$f" ] || continue
    echo "$f"
  done
}

normalize_minishell_output() {
  sed -E 's/^minishell > //; s/^> //; /^exit$/d; /^\+ /d'
}

normalize_minishell_error() {
  sed -E 's/^minishell > //; s/^> //; /^exit$/d; /^\+ /d'
}

normalize_bash_error() {
  sed -E 's/^\/bin\/bash:/minishell:/; s/: line [0-9]+: /: /; /^\+ /d'
}

compact_diff() {
  awk '
    FNR==NR { a[FNR]=$0; na=FNR; next }
    { b[FNR]=$0; nb=FNR }
    END {
      n = (na>nb)?na:nb
      for (i=1;i<=n;i++) {
        la = (i in a)?a[i]:"(missing)"
        lb = (i in b)?b[i]:"(missing)"
        if (la != lb) {
          print "bash: " la
          print "mini: " lb
        }
      }
    }
  ' "$1" "$2"
}

run_with_timeout() {
  local seconds="$1"
  shift
  perl -e 'alarm shift; exec @ARGV' "$seconds" "$@"
  local status=$?
  if [ "$status" -eq 142 ] || [ "$status" -eq 124 ]; then
    return 124
  fi
  return "$status"
}

