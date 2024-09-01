#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void handle_signal(int signal)
{
    rl_replace_line("", 0);
    rl_redisplay();
}

int main()
{
    // Register the signal handler (example for SIGINT)
    signal(SIGINT, handle_signal);

    // Example readline loop
    char *input;
    while ((input = readline("prompt> ")) != NULL)
    {
        if (*input)
        {
            add_history(input);
        }
        printf("You entered: %s\n", input);
        free(input);
    }

    return 0;
}
