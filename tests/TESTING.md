# Minishell Testing Guide

## 🚀 Quick Start

### Run Showcase Demo
```bash
./minishell < tests/cases/showcase.ms
```

### Run All Tests
```bash
bash tests/tester_cases.sh      # Functional tests (stdout/stderr)
bash tests/tester_exitcode.sh    # Exit code validation
bash tests/tester_fd.sh          # File descriptor leak check
```

---

## 📋 Test Cases

| File | Description |
|------|-------------|
| `01_basic.ms` | Basic commands (echo, pwd, env) |
| `02_builtins.ms` | Built-in commands (cd, export, unset, exit) |
| `03_quotes.ms` | Single/double/multiline quotes |
| `04_redirections.ms` | Input/output/append redirections |
| `05_pipes.ms` | Pipe chains (2-10 stages) |
| `06_heredoc.ms` | Heredoc with variable expansion |
| `07_herestring.ms` | Herestring (`<<<`) |
| `08_logic_ops.ms` | Logic operators (`&&`, `||`) |
| `09_exit_codes.ms` | Exit code handling |
| `10_edge_cases.ms` | Edge cases and error handling |
| `11_comprehensive.ms` | Integration test (all features) |
| `showcase.ms` | Portfolio demo |
| `fd_management.ms` | File descriptor management |

---

## 🧪 Test Suites

### 1. Functional Tests (`tester_cases.sh`)
Compares minishell output with bash (stdout/stderr).
- Reports saved to: `tests/result_cases.ms`

### 2. Exit Code Tests (`tester_exitcode.sh`)
Validates exit codes per command.
- Reports saved to: `tests/result_exitcode.ms`

### 3. File Descriptor Tests (`tester_fd.sh`)
Checks for FD leaks and detects hangs (timeout detection).
- Reports saved to: `tests/result_fd.ms`

---

## 🔍 Manual Testing

### Interactive Mode
```bash
./minishell
```

### Memory Leak Check (Valgrind)
```bash
bash tests/valgrind.sh
```

This script builds and runs Valgrind in Docker. For manual testing:
```bash
docker build -t minishell .
docker run --rm minishell < tests/cases/showcase.ms
```

### Check File Descriptors
```bash
./minishell &
PID=$!
lsof -p $PID
kill $PID
```

---

## 📊 Expected Results

All tests should pass with:
- ✅ Correct stdout/stderr output (matches bash)
- ✅ Correct exit codes
- ✅ No file descriptor leaks
- ✅ No memory leaks (valgrind clean)

