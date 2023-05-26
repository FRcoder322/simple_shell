#include "shell.h"

/**
 *  hsh - Main shell loop.
 * @av: Argument vector from main().
* @info: Parameter & return info structure.
 * Return: 0 on success, 1 on error, or error code
 */

int  hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		info_clear(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			info_set(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_comd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		info_free(info, 0);
	}
	write_history(info);
	info_free(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - Finds a builtin command.
 * @info: Parameter & return info structure.
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */

int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", exiting},
		{"env", _env},
		{"help", help_pro},
		{"history", history},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", cd},
		{"alias", alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_comd - Finds  command  PATH
 * @info: Parameter & return info structure.
 * Return: void
 */

void find_comd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!delim_is(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_comd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_comd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			error_print(info, "not found\n");
		}
	}
}

/**
 * fork_comd - Forks  exec thread to run comd.
 * @info: Parameter & return info structure
 * Return: void
 */

void fork_comd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{

		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			info_free(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}

	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
			error_print(info, "Permission denied\n");
		}
	}
}
