#include "shell.h"

/**
 * my_getenv - Retrieves the value of the specified environment variable.
 * @info: Structure containing potential arguments
 * @name: The name of the environment variable to retrieve.
 *
 * Return:
 *     - If the environment variable is found, returns a pointer to its value.
 *     - If the environment variable is not found, returns NULL.
 */

char *my_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	size_t name_len;
	char *str;

	while (node)
	{
		str = node->str;
		str_len = my_strlen(name);

		if (my_strncmp(str, name, name_len) == 0 && str[name_len] == '=')
		{
			return (node->str + name_len + 1);
		}
		node = node->next;
	}
	return (NULL);
}
