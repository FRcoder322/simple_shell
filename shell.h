#ifndef _SHELL_H_
#define _SHELL_H_

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

#define CMD_OR		1
#define READ_BUF_SIZE 1024
#define CMD_CHAIN	3
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM	0
#define HIST_MAX	4096
#define CONVERT_LOWERCASE	1
#define USE_STRTOK 0
#define CMD_AND		2
#define USE_GET_LINE 0
#define CONVERT_UNSIGNED	2
#define HIST_FILE	".simple_shell_history"


extern char **environ;


/**
 * struct liststr - Singly linked list
 * @num: Number field
 * @next: Points to the next node
* @str: String
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - Contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
*@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
  *@cmd_buf_type: CMD_type ||, &&, ;
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 *@line_count: the error count
  *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
*@arg: a string generated from getline containing arguements
 *@cmd_buf: address of pointer to cmd_buf, on if chaining


 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - Contains a builtin string and related function
 *@func: Function
 *@type: Builtin command flag

 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

int interactive(info_t *info);
int  delim_is(char c, char *delim);
int  alpha(int c);
int  _atoi(char *s);
int  hsh(info_t *info, char **av);
int find_builtin(info_t *info);
void find_comd(info_t *info);
void fork_comd(info_t *info);
char *_strncopy(char *dest, char *src, int n);
char *_strncat(char *dest, char *src, int n);
char *strchar(char *s, char c);
int main(int ac, char **av);
char **get_environ(info_t *info);
int _unsetenv(info_t *info, char *var);
int _setenv(info_t *info, char *var, char *value);
int _env(info_t *info);
char *_getenv(info_t *info, const char *name);
int _mysetenv(info_t *info);
int _myunsetenv(info_t *info);
int populate_env_list(info_t *info);
void info_clear(info_t *info);
void info_set(info_t *info, char **av);
void info_free(info_t *info, int all);
int exiting(info_t *info);
int cd(info_t *info);
int help_pro(info_t *info);
int history(info_t *info);
int alias_unset(info_t *info, char *str);
int set_alias(info_t *info, char *str);
int print_alias(list_t *node);
int alias(info_t *info);
void eputs(char *str);
int _eputchar(char c);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);
int erratoi(char *s);
void error_print(info_t *info, char *estr);
int print_d(int input, int fd);
char *conv_number(long int num, int base, int flags);
void rem_comments(char *buf);
ssize_t buffer_input(info_t *info, char **buf, size_t *len);
ssize_t get_input(info_t *info);
ssize_t read_buf(info_t *info, char *buf, size_t *i);
int get_line(info_t *info, char **ptr, size_t *length);
void sigintHandler(__attribute__((unused))int sig_num);
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);

int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);
int is_chain(info_t *info, char *buf, size_t *p);
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len);
int replace_alias(info_t *info);
int replace_vars(info_t *info);
int replace_string(char **old, char *new);
char *_memset(char *s, char b, unsigned int n);
void ffree(char **pp);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int bfree(void **ptr);
list_t *add_node(list_t **head, const char *str, int num);
list_t *add_node_end(list_t **head, const char *str, int num);
size_t print_list_str(const list_t *h);
int delete_node_at_index(list_t **head, unsigned int index);
void free_list(list_t **head_ptr);
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *haystack, const char *needle);
char *_strcat(char *dest, char *src);
int is_cmd(info_t *info, char *path);
char *dup_chars(char *pathstr, int start, int stop);
char *find_path(info_t *info, char *pathstr, char *cmd);
char *_strcopy(char *dest, char *src);
char *_strdup(const char *str);
void _puts(char *str);
int _putchar(char c);
char **strtoke(char *str, char *d);
char **strtoke2(char *str, char d);
size_t list_len(const list_t *h);
char **list_to_strings(list_t *head);
size_t print_list(const list_t *h);
list_t *node_starts_with(list_t *node, char *prefix, char c);
ssize_t get_node_index(list_t *head, list_t *node);

#endif
