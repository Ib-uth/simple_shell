#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **parse_arguments(char *line)
{
    int bufsize = MAX_ARGUMENTS;
    int position = 0;
    char **arguments = malloc(bufsize * sizeof(char *));
    char *argument;

    if (!arguments)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    argument = strtok(line, " \t\n\r\a");
    while (argument != NULL)
    {
        arguments[position] = argument;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGUMENTS;
            arguments = realloc(arguments, bufsize * sizeof(char *));
            if (!arguments)
            {
                perror("Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }

        argument = strtok(NULL, " \t\n\r\a");
    }
    arguments[position] = NULL;
    return arguments;
}

int execute_command(char **arguments)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(arguments[0], arguments) == -1)
        {
            perror("Error");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Forking error
        perror("Error");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int main(void)
{
    char *line;
    char **arguments;
    int status;

    printf("Simple Shell\n");
    printf("Type 'exit' to quit.\n");

    do
    {
        printf("> ");
        line = read_line();
        arguments = parse_arguments(line);
        status = execute_command(arguments);

        free(line);
        free(arguments);
    } while (status);

    return 0;
}
