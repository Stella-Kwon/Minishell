
#### MINISHELL

## Supported Features

- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Unlimited pipelines: `cmd1 | cmd2 | cmd3 ...`
- Logical operators: `&&`, `||` with short-circuit evaluation
- Redirections: `<`, `>`, `>>`, `<<` (heredoc), `<<<` (herestring)
- Quote-aware parsing (single & double quotes)
- Variable expansion: `$VAR`, `$?`
- Context-aware signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- Correct stdin precedence when combining heredoc and herestring

---

## How It Works

Input is tokenized, expanded, and transformed into an **Abstract Syntax Tree (AST)**.  
Commands are executed through recursive tree traversal, which naturally enforces
operator precedence (`|` > `&&` > `||`) and correct short-circuit behavior.

Stdin sources are explicitly tracked to resolve precedence between `<`, `<<`, and
`<<<`, matching Bash behavior in complex redirection scenarios.

---

## Code Structure

srcs/
├── tokenize/ # Lexical analysis
├── parsing/ # AST construction
├── execution/ # fork/exec, pipelines, FD control
├── builtin/ # Built-in commands
├── expand/ # Variable & quote expansion
├── heredoc_string/ # Heredoc & herestring handling
├── signal/ # Signal configuration
├── error_log/ # Error handling
└── utils/ # Memory-safe utilities

---

### Run All Tests
```bash
bash tests/tester_cases.sh      # Functional tests (stdout/stderr)
bash tests/tester_exitcode.sh    # Exit code validation
bash tests/tester_fd.sh          # File descriptor leak check
bash test/valgrind.sh           # Memory-leak check
```
---

## 📋 Test Cases

| File                  | Description                                         |   |    |
| --------------------- | --------------------------------------------------- | - | -- |
| `01_basic.ms`         | Basic commands (`echo`, `pwd`, `env`)               |   |    |
| `02_builtins.ms`      | Built-in commands (`cd`, `export`, `unset`, `exit`) |   |    |
| `03_quotes.ms`        | Single, double, and multiline quotes                |   |    |
| `04_redirections.ms`  | Input/output/append redirections                    |   |    |
| `05_pipes.ms`         | Pipe chains (2–10 stages)                           |   |    |
| `06_heredoc.ms`       | Heredoc with variable expansion                     |   |    |
| `07_herestring.ms`    | Herestring (`<<<`)                                  |   |    |
| `08_logic_ops.ms`     | Logical operators (`&&`, `                          |   | `) |
| `09_exit_codes.ms`    | Exit code propagation                               |   |    |
| `10_edge_cases.ms`    | Error handling and edge cases                       |   |    |
| `11_comprehensive.ms` | Full integration test                               |   |    |
| `fd_management.ms`    | File descriptor management                          |   |    |

---

## Recent Improvements

- Fixed variable expansion in double-quoted herestrings
- Refactored heredoc and herestring handling to run in the main process  
  (removed unnecessary `fork()` usage)
- Corrected stdin precedence between heredoc and herestring  
  (e.g. `cat << LIMITER <<< "hi"`)
- Fixed exit code propagation for `exit` without arguments
- Added dual input handling to support both interactive and non-interactive modes  
  (`readline()` for TTY input, `getline()` for static file / tester input)
- Reworked signal handling after removing `fork()` by using `sigsetjmp` / `siglongjmp`  
  to restore execution state and safely abort the current command within a single process

---

## Authors

**skwon2** — Hive Helsinki (42 Network)

**hlee-sun** — Hive Helsinki (42 Network)

---

Developed as part of the 42 Network curriculum at Hive Helsinki.
