#include "shell.h"

/**
 * erratoi - Converts astring to int.
 * @s: String to be converted.
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */

int erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * error_print - Prints  error message.
* @estr: String cotains specified error.
 * @info: Parameter & return info struct
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */

void error_print(info_t *info, char *estr)
{
	eputs(info->fname);
	eputs(": ");
	print_d(info->line_count, STDERR_FILENO);
	eputs(": ");
	eputs(info->argv[0]);
	eputs(": ");
	eputs(estr);
}

/**
 * print_d - Fun prints adec (integer) number (base 10).
* @fd: Filedescriptor to write.
 * @input:  Input
 * Return: number of characters printed
 */

int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * conv_number - Converter fun clone of itoa.
* @base: base
 * @flags: Argument flags
 * @num: Number
 * Return: String.
 */

char *conv_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * rem_comments - Fun replace first instance of '#' with '\0'
 * @buf: Address of string to modify.
 * Return: Always 0;
 */

void rem_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
