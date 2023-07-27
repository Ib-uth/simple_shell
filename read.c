#include "shell.h"

/**
 * read_command - Read input command from standard input
 *
 * Return: Pointer to the input command or NULL on end-of-file (Ctrl+D)
 */

char *read_command(void)
{
	char *command = NULL;
	size_t command_len = 0;

	if (getline(&command, &command_len, stdin) == -1)
	{
		write_stdout("\n"); /* End of file condition (Ctrl+D) */
		return (NULL);
	}
	command[my_strcspn(command, "\n")] = '\0'; /* Remove trail newline charact */
	return (command);
}
