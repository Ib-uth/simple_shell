#include "shell.h"

/**
 * find_command_path - Searches for the given command within the provided path.
 * @command: The command to find.
 * @path: The path to search within.
 *
 * Return: On success, returns the path to the command. If the command is not
 *         found or an error occurs, returns NULL.
 */

char *find_command_path(char *command, char *path)
{
	char *path_token;
	char *path_entry;
	char *command_path;

	path_token = strtok(path, ":");
	while (path_token != NULL)
	{
		size_t path_len = my_strlen(path_token);
		size_t command_len = my_strlen(command);

		path_entry = malloc((path_len + command_len + 2) * sizeof(char));

		if (path_entry == NULL)
		{
			write_stderr("Memory allocation failed\n");
			break;
		}
		my_strcpy(path_entry, path_token);
		my_strcat(path_entry, "/");
		my_strcat(path_entry, command);

		/* Check if the file exists */
		if (access(path_entry, F_OK) == 0)
		{
			command_path = path_entry;
			break;
		}
		free(path_entry);
	}
	return (command_path);
}

/**
 * get_command_path - Retrieves the full path of a command from
 * the PATH environment variable.
 * @command: The command name.
 *
 * Return: The full path of the command, or NULL if not found.
 */
char *get_command_path(char *command)
{
	char *path;
	char *command_path;

	path = my_getenv("PATH");

	if (command == NULL || path == NULL)
	{
		return (NULL);
	}
	command_path = find_command_path(command, path);
	/* free(path);*/
	return (command_path);
}
