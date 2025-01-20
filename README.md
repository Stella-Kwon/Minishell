Minishell
Description
Minishell is a simple shell implementation that mimics the behavior of a Unix shell. It features basic functionalities such as handling commands, redirections, pipes, environment variables, and built-ins. It also supports custom signal handling for interactive sessions, and implements several fundamental shell operations like echo, cd, pwd, export, unset, env, and exit.

Mandatory Requirements
Features
Prompt & History: The shell should display a prompt when waiting for new commands. It also has a history feature, where commands can be recalled.

Search & Launch: The shell must search for executables based on the $PATH variable or use relative/absolute paths to launch programs.

Signal Handling: Avoid using global variables for signal handling. Only use a global variable to store the signal number, without accessing other data structures.

Quoting & Special Characters:

Single quotes (') prevent the interpretation of metacharacters inside the quotes.
Double quotes (") prevent interpretation except for the dollar sign ($).
Unclosed quotes or unnecessary special characters (e.g., semicolons or backslashes) should be ignored.
Redirection:

<: Redirects input.
>: Redirects output.
<<: Reads input until a line containing the delimiter is seen.
>>: Appends output to a file.
Pipes: The | operator connects the output of one command to the input of the next command.

Environment Variables: Expand variables like $VAR to their corresponding values from the environment. Handle the special case of $? for the exit status of the last executed foreground process.

Built-ins:

echo with the -n option.
cd for navigating directories (supports relative/absolute paths).
pwd for printing the current directory.
export to set environment variables.
unset to remove environment variables.
env to display environment variables.
exit to terminate the shell with no options.
Signal Handling: Handle Ctrl-C, Ctrl-D, and Ctrl-\ as follows:

Ctrl-C displays a new prompt on a new line.
Ctrl-D exits the shell.
Ctrl-\ has no effect.
External Functions Used
Your implementation should use the following external functions:

readline, rl_clear_history, add_history, printf, malloc, free, write, access, open, close, fork, execve, signal, kill, chdir, getcwd, stat, lstat, fstat, unlink, dup, dup2, pipe, opendir, readdir, closedir, getenv, tcsetattr, tcgetattr, and others for terminal management and system calls.
Libft
You are authorized to use your own libft for auxiliary functions like string manipulation, memory management, and others.

Bonus Requirements
The following additional features are optional but can enhance your shell implementation:

Logical Operators:
&&: Executes the second command only if the first command succeeds (exit status 0).
||: Executes the second command only if the first command fails (non-zero exit status).
Files to Submit
Makefile: Must include targets: NAME, all, clean, fclean, and re.
Source Files: All your .c files that make up the shell implementation.
Header Files: All your .h header files.
Compilation
To compile the Minishell program, use the following command:

bash
Copy
Edit
make
This will compile the program and generate the minishell binary.

Cleanup
To clean up all object files and temporary files, use:

bash
Copy
Edit
make clean
For a complete cleanup, including the removal of the minishell binary, use:

bash
Copy
Edit
make fclean
To recompile everything from scratch, use:

bash
Copy
Edit
make re
Known Issues & Limitations
The program does not fix memory leaks in readline().
The behavior of special characters like backslashes and semicolons is intentionally limited.
Only a single signal number is available globally for signal handling.
Acknowledgements
This project was completed as part of the 42 Network curriculum.

