#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

typedef struct Alias {
    char *name;
    char *value;
    struct Alias *next;
} Alias;

Alias *alias_list = NULL;
int last_exit_status = 0;

char *read_line()
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char *read_line_from_file(FILE *file)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters_read;

    characters_read = getline(&line, &bufsize, file);
    if (characters_read == -1)
    {
        free(line);
        return NULL; // End of file or error
    }

    // Remove the trailing newline character, if present
    if (line[characters_read - 1] == '\n')
    {
        line[characters_read - 1] = '\0';
    }

    return line;
}

char **parse_arguments(char *line)
{
    int bufsize = MAX_ARGUMENTS;
    int position = 0;
    char **arguments = (char **)malloc(bufsize * sizeof(char *));
    char *argument;

    if (!arguments)
    {
        perror("Memory allocation error");
        _exit(EXIT_FAILURE);
    }

    argument = strtok(line, " \t\n\r\a");
    while (argument != NULL)
    {
        // Check for comments (ignore anything after #)
        if (argument[0] == '#')
        {
            break;
        }

        arguments[position] = argument;
        position++;

        if (position >= bufsize)
        {
            bufsize += MAX_ARGUMENTS;
            arguments = (char **)realloc(arguments, bufsize * sizeof(char *));
            if (!arguments)
            {
                perror("Memory allocation error");
                _exit(EXIT_FAILURE);
            }
        }

        argument = strtok(NULL, " \t\n\r\a");
    }
    arguments[position] = NULL;
    return arguments;
}

Alias *find_alias(char *name)
{
    Alias *current = alias_list;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print_aliases()
{
    Alias *current = alias_list;
    while (current != NULL)
    {
        write(STDOUT_FILENO, current->name, strlen(current->name));
        write(STDOUT_FILENO, "='", 2);
        write(STDOUT_FILENO, current->value, strlen(current->value));
        write(STDOUT_FILENO, "'\n", 2);
        current = current->next;
    }
}

void print_alias(char *name)
{
    Alias *alias = find_alias(name);
    if (alias != NULL)
    {
        write(STDOUT_FILENO, alias->name, strlen(alias->name));
        write(STDOUT_FILENO, "='", 2);
        write(STDOUT_FILENO, alias->value, strlen(alias->value));
        write(STDOUT_FILENO, "'\n", 2);
    }
}

void set_alias(char *name, char *value)
{
    Alias *alias = find_alias(name);
    if (alias != NULL)
    {
        // If the alias already exists, replace its value
        free(alias->value);
        alias->value = strdup(value);
    }
    else
    {
        // If the alias doesn't exist, create a new one
        Alias *new_alias = (Alias *)malloc(sizeof(Alias));
        if (new_alias == NULL)
        {
            perror("Memory allocation error");
            _exit(EXIT_FAILURE);
        }
        new_alias->name = strdup(name);
        new_alias->value = strdup(value);
        new_alias->next = alias_list;
        alias_list = new_alias;
    }
}

void execute_alias_command(char **arguments)
{
    if (arguments[1] == NULL)
    {
        // Print all aliases if no arguments are given
        print_aliases();
    }
    else
    {
        int i = 1;
        while (arguments[i] != NULL)
        {
            if (strchr(arguments[i], '='))
            {
                // If the argument contains '=', it's an alias definition
                char *name = strtok(arguments[i], "=");
                char *value = strtok(NULL, "=");
                if (name != NULL && value != NULL)
                {
                    set_alias(name, value);
                }
                else
                {
                    char *error_msg = "Invalid alias format: ";
                    write(STDERR_FILENO, error_msg, strlen(error_msg));
                    write(STDERR_FILENO, arguments[i], strlen(arguments[i]));
                    write(STDERR_FILENO, "\n", 1);
                }
            }
            else
            {
                // Otherwise, it's an alias name to print
                print_alias(arguments[i]);
            }
            i++;
        }
    }
}

int execute_command(char **arguments, char *path)
{
    char *full_path;
    pid_t pid, wpid;
    int status;

    if (strcmp(arguments[0], "exit") == 0)
    {
        if (arguments[1] != NULL)
        {
            int exit_status = atoi(arguments[1]);
            _exit(exit_status);
        }
        else
        {
            _exit(0);
        }
    }
    else if (strcmp(arguments[0], "setenv") == 0)
    {
        if (arguments[1] == NULL || arguments[2] == NULL)
        {
            char *usage_msg = "Usage: setenv VARIABLE VALUE\n";
            write(STDERR_FILENO, usage_msg, strlen(usage_msg));
            return 1;
        }
        if (setenv(arguments[1], arguments[2], 1) == -1)
        {
            perror("Error setting environment variable");
            return 1;
        }
        return 1;
    }
    else if (strcmp(arguments[0], "unsetenv") == 0)
    {
        if (arguments[1] == NULL)
        {
            char *usage_msg = "Usage: unsetenv VARIABLE\n";
            write(STDERR_FILENO, usage_msg, strlen(usage_msg));
            return 1;
        }
        if (unsetenv(arguments[1]) == -1)
        {
            perror("Error unsetting environment variable");
            return 1;
        }
        return 1;
    }
    else if (strcmp(arguments[0], "cd") == 0)
    {
        char *dir;
        if (arguments[1] == NULL || strcmp(arguments[1], "~") == 0)
        {
            // If no argument is given or if "~" is given, change to $HOME directory
            dir = getenv("HOME");
            if (dir == NULL)
            {
                char *error_msg = "cd: No HOME environment variable set\n";
                write(STDERR_FILENO, error_msg, strlen(error_msg));
                return 1;
            }
        }
        else if (strcmp(arguments[1], "-") == 0)
        {
            // Change to the previous directory
            dir = getenv("OLDPWD");
            if (dir == NULL)
            {
                char *error_msg = "cd: No OLDPWD environment variable set\n";
                write(STDERR_FILENO, error_msg, strlen(error_msg));
                return 1;
            }
        }
        else
        {
            dir = arguments[1];
        }

        char current_dir[4096];
        if (getcwd(current_dir, sizeof(current_dir)) == NULL)
        {
            perror("getcwd");
            return 1;
        }

        if (chdir(dir) != 0)
        {
            perror("cd");
            return 1;
        }

        // Update PWD and OLDPWD environment variables
        if (setenv("OLDPWD", current_dir, 1) == -1)
        {
            perror("Error setting OLDPWD environment variable");
            return 1;
        }

        char new_dir[4096];
        if (getcwd(new_dir, sizeof(new_dir)) == NULL)
        {
            perror("getcwd");
            return 1;
        }
        if (setenv("PWD", new_dir, 1) == -1)
        {
            perror("Error setting PWD environment variable");
            return 1;
        }

        return 1;
    }

    // Handle variable replacement
    for (int i = 0; arguments[i] != NULL; i++)
    {
        char *arg = arguments[i];
        char *replaced_arg = NULL;

        if (strcmp(arg, "$?") == 0)
        {
            // Replace $? with the exit status of the last command
            asprintf(&replaced_arg, "%d", last_exit_status);
        }
        else if (strcmp(arg, "$$") == 0)
        {
            // Replace $$ with the process ID of the shell
            asprintf(&replaced_arg, "%d", getpid());
        }

        if (replaced_arg != NULL)
        {
            free(arguments[i]);
            arguments[i] = replaced_arg;
        }
    }

    if (strcmp(arguments[0], "alias") == 0)
    {
        execute_alias_command(arguments);
        return 1;
    }

    if (strcmp(arguments[0], "env") == 0)
    {
        char **env_var = environ;
        while (*env_var)
        {
            write(STDOUT_FILENO, *env_var, strlen(*env_var));
            write(STDOUT_FILENO, "\n", 1);
            env_var++;
        }
        return 1;
    }

    full_path = search_command(arguments[0], path);
    if (full_path == NULL)
    {
        char *error_msg = "command not found: ";
        write(STDERR_FILENO, error_msg, strlen(error_msg));
        write(STDERR_FILENO, arguments[0], strlen(arguments[0]));
        write(STDERR_FILENO, "\n", 1);
        return 1;
    }

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execve(full_path, arguments, environ) == -1)
        {
            perror("Error");
        }
        _exit(EXIT_FAILURE);
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

    free(full_path);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int execute_multiple_commands(char *line, char *path)
{
    char *command;
    char *saveptr;

    command = strtok_r(line, ";", &saveptr);
    while (command != NULL)
    {
        int should_execute = 1;
        char **arguments;

        // Check if the command contains '&&' or '||'
        if (strstr(command, "&&"))
        {
            arguments = parse_arguments(command);
            for (int i = 0; arguments[i] != NULL; i++)
            {
                if (strcmp(arguments[i], "&&") == 0)
                {
                    should_execute = should_execute && (execute_command(arguments, path) == 0);
                }
            }
        }
        else if (strstr(command, "||"))
        {
            arguments = parse_arguments(command);
            for (int i = 0; arguments[i] != NULL; i++)
            {
                if (strcmp(arguments[i], "||") == 0)
                {
                    should_execute = should_execute && (execute_command(arguments, path) != 0);
                }
            }
        }
        else
        {
            arguments = parse_arguments(command);
            should_execute = execute_command(arguments, path) == 0;
        }

        free(arguments);

        if (!should_execute)
        {
            break; // Stop executing further commands on false condition
        }

        command = strtok_r(NULL, ";", &saveptr);
    }
}

int main(int argc, char *argv[])
{
    char *line;
    int status = 1;
    int read_from_file = 0; // Set to 1 if reading from a file
    FILE *file = NULL;

    write(STDOUT_FILENO, "Simple Shell\n", 13);

    if (argc > 1)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            perror("Error opening file");
            return 1;
        }
        read_from_file = 1;
    }

    char *path = getenv("PATH");
    if (path == NULL)
    {
        perror("Error getting PATH environment variable");
        return 1;
    }

    do
    {
        if (read_from_file)
        {
            line = read_line_from_file(file);
            if (line == NULL)
            {
                break;
            }
        }
        else
        {
            write(STDOUT_FILENO, "> ", 2);
            line = read_line();
        }

        status = execute_multiple_commands(line, path);

        free(line);
    } while (status);

    if (read_from_file)
    {
        fclose(file);
    }

    return 0;
}
