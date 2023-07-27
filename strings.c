#include "shell.h"

/**
 * my_strlen - Calculate the length of a string.
 * @str: The input string.
 *
 * Return: The length of the string.
 */
size_t my_strlen(const char *str)
{
	size_t len = 0;

	while (str[len] != '\0')
	{
		len++;
	}

	return (len);
}

/**
 * my_strcpy - Copy a string from source to destination.
 * @destination: The destination string.
 * @source: The source string.
 *
 * Description: This function copies the string from source to destination,
 * including the null-terminating character. The destination
 * string must have enough space to accommodate the source string.
 */

void my_strcpy(char *destination, const char *source)
{
	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;

		*destination = '\0';
	}
}

/**
 * write_status_message - Writes a status message to standard output.
 * @status: The exit status.
 */

void write_status_message(int status)
{
	char status_msg[32];
	char exit_code[2];

	exit_code[0] = '0' + WEXITSTATUS(status);
	exit_code[1] = '\0';

	my_strcpy(status_msg, "Child process exited with status ");
	my_strcpy(status_msg + my_strlen(status_msg), exit_code);
	my_strcpy(status_msg + my_strlen(status_msg), "\n");

	write_stdout(status_msg);
}

/**
 * my_strcmp - Compare two strings lexicographically.
 * @str1: The first string.
 * @str2: The second string.
 *
 * Return: An integer less than, equal to, or greater than zero if str1 is
 * found, respectively, to be less than, to match, or be greater than str2.
 */

int my_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

