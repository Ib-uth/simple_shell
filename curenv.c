#include "shell.h"

/**
 * print_environment - Prints the current environment variables.
 *
 * This function iterates over the `environ` array, which contains the current
 * environment variables, and prints each variable followed by a newline.
 */

int print_environment(info_t *info)
{
	print_list(info->env);
	return (0);
}

/**
 * print_list - prints only the str element of a list_t linked list.
 * @ptr: pointer to first node.
 *
 * Return: size of list.
 */
size_t print_list(const list_t *ptr)
{
	size_t i = 0;

	while (ptr)
	{
		write_stdout(ptr->str ? ptr->str : "(nil)");
		write_stdout("\n");
		ptr = ptr->next;
		i++;
	}
	return (i);
}
