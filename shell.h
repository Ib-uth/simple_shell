#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <stddef.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
#define MAX_SIZE_DIGITS 20
#define MAX_PATH_LENGTH 1024
#define MAX_FILES 100

struct list_s
{
    char *str;
    struct list_s *next;
};

typedef struct list_s list_t;
struct info_s
{
	list_t *env;
};
typedef struct info_s info_t;



char *my_getenv(info_t *info, const char *name);




void write_stdout(const char *msg);
void write_stderr(const char *msg);
void write_status_message(int status);

char *get_command_path(char *command);

size_t my_strlen(const char *str);
void my_strcpy(char *destination, const char *source);
char *my_strcat(char *destination, const char *source);
int my_strcmp(const char *str1, const char *str2);
size_t my_strcspn(const char *str1, const char *str2);
int my_strncmp(const char *str1, const char *str2, size_t n);

void tokenize_arguments(char *command, char *arguments[]);
char *my_strncat(char *dest, const char *src, size_t n);
int execute_command(char *arguments[]);
void handle_command(char *command);
void print_path_environment_variable(void);
char *find_command_path(char *command, char *path);
void list_files(void);
char *read_command(void);
void write_stdout_char(char c);

void sort_filenames(char *filenames[], int num_files);

int print_environment(info_t *info);
size_t print_list(const list_t *ptr);
#endif
