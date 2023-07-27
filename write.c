#include "shell.h"

/**
 * write_stdout - Writes a message to standard output.
 * @msg: The message to write.
 */

void write_stdout(const char *msg)
{
	write(STDOUT_FILENO, msg, my_strlen(msg));
}

/**
 * write_stderr - Writes a message to standard error.
 * @msg: The message to write.
 */

void write_stderr(const char *msg)
{
	write(STDERR_FILENO, msg, my_strlen(msg));
}

/**
 * write_stdout_char - Writes a single character to the standard output
 *
 * @c: The character to be written to the standard output.
 */
void write_stdout_char(char c)
{
	write(1, &c, 1);
}
