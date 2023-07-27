#include "shell.h"

/**
 * tokenize_arguments - Tokenizes a command line into arguments.
 * @command: The command line string to be tokenized.
 * @arguments: An array to store the resulting arguments.
 *
 * Description:
 *   This function takes a command line string and tokenizes it into separate
 *   arguments based on space (' ') delimiters. The resulting arguments are
 *   stored in the provided `arguments` array, with a maximum number of
 *   arguments limited by `MAX_ARGUMENTS`. The last element of the `arguments`
 *   array is set to NULL to indicate the end of the argument list.
 */

void tokenize_arguments(char *command, char *arguments[])
{
	char *token;
	int arg_count = 0;

	token = strtok(command, " ");
	while (token != NULL && arg_count < MAX_ARGUMENTS)
	{
		arguments[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " ");
	}
	arguments[arg_count] = NULL;
}

/**
 * execute_command - Executes a command by forking a child process.
 * @arguments: An array of command arguments.
 *
 * Description: his function forks a child process and executes a command
 * with the given arguments
 *
 *  Return: Returns 0 upon successful execution.
 */

int execute_command(char *arguments[])
{
	int status;
	pid_t pid = fork();

	if (pid < 0)
	{
		write_stderr("fork failed\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		execve(arguments[0], arguments, NULL);
		perror("");
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Parent process */
		waitpid(pid, &status, 0);
		/* write_status_message(status); */
	}
	return (0);
}

/**
 * handle_command - Handle a single command entered by the user.
 * @command: The command to handle.
 *
 * This function tokenizes the command into arguments and executes the command
 * if it is not an exit command.
 */

void handle_command(char *command)
{
	char *arguments[MAX_ARGUMENTS + 1]; /* Additional space for NULL */

	if (command == NULL)
	{
		write_stderr("Invalid command\n");
		return;
	}

	tokenize_arguments(command, arguments);

	if (arguments[0] != NULL)
	{
		if (my_strcmp(arguments[0], "exit") == 0)
		{
			write_stdout("\n");
			return; /* Exit the shell */
		}

		execute_command(arguments);
	}
}

/**
 * print_path_environment_variable - Prints the value of the PATH
 * environment variable.
 * This function retrieves the value of the PATH environment variable using
 * the my_getenv function.
 * If the value is found, it is printed to the standard output along
 * with a descriptive message.
 */

void print_path_environment_variable(void)
{
	const char *variable_name = "PATH";
	char *value = my_getenv(variable_name);

	if (value != NULL)
	{
		write_stdout("Value of ");
		write_stdout(variable_name);
		write_stdout(": ");
		write_stdout(value);
		write_stdout("\n");
	}
	else
	{
	    write_stdout("Environment variable ");
	    write_stdout(variable_name);
	    write_stdout(" n not found\n");
	}
}

/**
 * main - Entry point of the shell program.
 *
 * Return: Always 0.
 */

int main(int argc, char *argv[], char *env[])
{
	info_t info;
	char *command = NULL;
	size_t command_len;/* = my_strlen(command); */
	char *command_path;

	while (1)
	{
		write_stdout("$ ");
		command = read_command();
		if (command == NULL)
		break;
		command_len = my_strlen(command);
		if (command_len == 0) /* Check if input is empty */
		continue;
		if (my_strcmp(command, "exit") == 0)
		{
			break; /* Terminate the shell loop */}
		else if (my_strcmp(command, "env") == 0)
		{
			write_stdout("\n");
			print_environment(info.env);
			continue; /* Continue to the next iteration of the loop */
		}
		else if (my_strcmp(command, "ls") == 0)
		{
			list_files();
			free(command); /* Call the list_files() display the cd */
			continue;
		}
		handle_command(command);
		command_path = get_command_path(command); /* Get the command path */
		if (command_path == NULL)
		{
			continue;  /* Continue to the next iteration of the loop */
		}
		free(command);
		print_path_environment_variable();
		free(command);
	}
	return (0);
}
