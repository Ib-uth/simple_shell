#include "shell.h"
#include <time.h>

#define MAX_FILES 100
/**
 * list_files - Lists files in a directory
 */

void list_files(void)
{
	DIR *dir;
	struct dirent *entry;
	char *filenames[MAX_FILES];
	int num_files = 0;
	int i;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror("opendir");
		return;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		/* Skip the dots (current directory and parent directory) */
		if (my_strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		if (num_files < MAX_FILES)
		{
			filenames[num_files] = entry->d_name;
			num_files++;
		}
	}
	closedir(dir);

	/* Bubble sort the filenames (simplest sorting algorithm) */
	sort_filenames(filenames, num_files);
	for (i = 0; i < num_files; i++)
	{
		write_stdout(filenames[i]);
		write_stdout("  ");
	}

	write_stdout("\n");
}
