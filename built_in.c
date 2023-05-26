#include "shell.h"

/**
 * _exiting - Exits the shell
 * @info: Structure contains potential arguments.
 *  Return: Exits with given exit status.
 *         (0) if info.argv[0] != "exit"
 */

int exiting(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  
	{
		exitcheck = erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			error_print(info, "Illegal number: ");
			eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * cd - Changes  current dir process.
 * @info: Structure contains potential arguments.
 *  Return: Always 0
 */

int cd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenv(info, "HOME=");
		if (!dir)
			chdir_ret =
				chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret =
			chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		error_print(info, "can't cd to ");
		eputs(info->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * help_pro - Changes current dir.
 * @info: Structure containing potential arguments.
 *  Return: Always 0
 */

int help_pro(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array);
	return (0);
}
