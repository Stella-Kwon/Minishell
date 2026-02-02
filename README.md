
# MINISHELL

## Demo

[![asciinema demo](https://asciinema.org/a/1WY066ZiBwrpoHLrsvg)](https://asciinema.org/a/1WY066ZiBwrpoHLr)


---

## Supported Features

- **Built-in commands**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Pipelines**: unlimited chaining (`cmd1 | cmd2 | cmd3 ...`)
- **Logical operators**: `&&`, `||` with proper short-circuit evaluation
- **Redirections**: `<`, `>`, `>>`, `<<` (heredoc), `<<<` (herestring)
- Quote-aware parsing (single & double quotes)
- Variable expansion: `$VAR`, `$?`
- Context-aware signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- ⭐ Correct stdin precedence when combining heredoc and herestring

---

## How It Works

Input is processed through a clearly defined execution pipeline:

1. **Tokenization & expansion**  
   Raw input is tokenized and expanded with respect to quotes and environment variables.

2. **AST construction**  
   Tokens are transformed into an **Abstract Syntax Tree (AST)** that encodes operator precedence.

3. **Recursive execution**  
   The AST is executed via recursive traversal, naturally enforcing precedence  
   (`|` > `&&` > `||`) and correct short-circuit behavior.

Stdin sources are explicitly tracked to resolve precedence between `<`, `<<`, and `<<<`,  
matching Bash behavior in complex redirection scenarios.

---

## Code Structure

```text
srcs/
├── tokenize/         # Lexical analysis (splitting input into tokens)
├── parsing/          # AST construction & operator precedence handling
├── execution/        # fork/exec, pipelines, and FD control
├── builtin/          # Built-in command implementations
├── expand/           # Variable & quote expansion
├── heredoc_string/   # Heredoc & herestring handling
├── signal/           # Signal configuration and recovery
├── error_log/        # Centralized error handling
└── utils/            # Memory-safe helper utilities
```

---

## Running Tests

```bash
bash tests/tester_cases.sh       # Functional tests (stdout / stderr)
bash tests/tester_exitcode.sh    # Exit code validation
bash tests/tester_fd.sh          # File descriptor leak checks
bash tests/valgrind.sh           # Memory leak detection
```
## Test Cases

| File                  | Description                                  |
| --------------------- | -------------------------------------------- |
| `01_basic.ms`         | Basic commands (`echo`, `pwd`, `env`)        |
| `02_builtins.ms`      | Built-in commands (`cd`, `export`, `exit`)   |
| `03_quotes.ms`        | Single, double, and multiline quotes         |
| `04_redirections.ms`  | Input/output/append redirections             |
| `05_pipes.ms`         | Pipe chains (2–10 stages)                    |
| `06_heredoc.ms`       | Heredoc with variable expansion              |
| `07_herestring.ms`    | Herestring (`<<<`)                           |
| `08_logic_ops.ms`     | Logical operators (`&&`, `||`)               |
| `09_exit_codes.ms`    | Exit code propagation                        |
| `10_edge_cases.ms`    | Error handling and edge cases                |
| `11_comprehensive.ms` | Full integration test                        |
| `fd_management.ms`    | File descriptor management                   |

---

## Recent Improvements

- Reworked signal handling after removing `fork()` by using  
  `sigsetjmp` / `siglongjmp` to safely abort execution within a single process
- Fixed variable expansion in double-quoted herestrings
- Refactored heredoc and herestring handling to run in the main process  
  (removed unnecessary `fork()` usage)
- Corrected stdin precedence between heredoc and herestring  
  (e.g. `cat << LIMITER <<< "hi"`)
- Fixed exit code propagation for `exit` without arguments
- Added dual input handling to support both interactive and non-interactive modes  
  (`readline()` for TTY input, `getline()` for static file / tester input)

---

## Authors

**skwon2** — Hive Helsinki (42 Network)  
**hlee-sun** — Hive Helsinki (42 Network)

---

Developed as part of the **42 Network curriculum** at Hive Helsinki.
