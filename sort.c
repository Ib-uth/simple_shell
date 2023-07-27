#include "shell.h"

/**
 * sort_filenames - Sorts the filenames array in ascending order
 * @filenames: The array of filenames to be sorted
 * @num_files: The number of filenames in the array
 *
 * Description: This function uses the Bubble Sort algorithm to
 * sort the filenames
 * array in ascending order based on their lexicographic order (ASCII values).
 */
void sort_filenames(char *filenames[], int num_files)
{
	int i, j;
	char *temp;

	/* Bubble sort the filenames in ascending order */
	for (i = 0; i < num_files - 1; i++)
	{
		for (j = 0; j < num_files - i - 1; j++)
		{
			if (my_strcmp(filenames[j], filenames[j + 1]) > 0)
			{
				/* Swap the filenames if they are in the wrong order */
				temp = filenames[j];
				filenames[j] = filenames[j + 1];
				filenames[j + 1] = temp;
			}
		}
	}
}
