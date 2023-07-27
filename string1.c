#include "shell.h"

/**
 * my_strcspn - Calculates the length of the initial segment of a string
 *              that does not contain any characters from another string.
 * @str1: The string to be searched.
 * @str2: The set of characters to search for.
 *
 * Return: The length of the initial segment without matching characters.
 */
size_t my_strcspn(const char *str1, const char *str2)
{
	const char *p1, *p2;

	for (p1 = str1; *p1 != '\0'; p1++)
	{
		for (p2 = str2; *p2 != '\0'; p2++)
		{
			if (*p1 == *p2)
			{
				return (p1 - str1);
			}
		}
	}

	return (p1 - str1);
}

/**
 * my_strcat - Concatenates two strings.
 * @destination: The destination string.
 * @source: The source string.
 *
 * Return: Pointer to the resulting concatenated string.
 */
char *my_strcat(char *destination, const char *source)
{
	char *ptr = destination;

	/* Find the end of the destination string */
	while (*ptr)
		ptr++;

	/* Append characters from the source string to the destination string */
	while (*source)
	{
		*ptr = *source;
		ptr++;
		source++;
	}

	/* Append null character at the end of the concatenated string */
	*ptr = '\0';

	return (destination);
}

/**
 * my_strncat - Concatenates at most 'n' characters from 'src' to 'dest'
 * @dest: The destination buffer to which 'src' will be appended
 * @src: The source string to be concatenated to 'dest'
 * @n: The maximum number of characters to concatenate from 'src'
 *
 * Return: A pointer to the destination buffer 'dest'
 */

char *my_strncat(char *dest, const char *src, size_t n)
{
	char *original_dest = dest;

	/* Move the destination pointer to the end of the destination string */
	while (*dest)
		dest++;

	/* Copy at most 'n' characters from src to dest */
	while (*src && n > 0)
	{
		*dest++ = *src++;
		n--;
	}

	/* Null-terminate the destination string */
	*dest = '\0';

	return (original_dest);
}

/**
 * my_strncmp - compare two strings up to a specified number of characters
 * @str1: pointer to the first string to compare
 * @str2: pointer to the second string to compare
 * @n: the maximum number of characters to compare
 *
 * Return: an integer less than, equal to, or greater than zero if the first n
 *         characters of str1 are found, respectively, to be less than, to
 *         match, or be greater than the first n characters of str2
 */
int my_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
	{
		if (str1[i] == '\0' || str2[i] == '\0' || str1[i] != str2[i])
			return str1[i] - str2[i];
	}

	return (0);
} 
