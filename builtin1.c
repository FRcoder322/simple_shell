#include "shell.h"

/**
 * history - Displays  history list,
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments.
 *  Return: Always 0
 */

int history(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * alias_unset - Sets alias  string.
* @str: String alias.
 * @info: Parameter struct.
 * Return: Always 0 on success, 1 on error
 */

int alias_unset(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = strchar(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - Sets alias .
 * @str: String alias.
 * @info: Parameter structure.
 * Return: Always 0 on success, 1 on error
 */

int set_alias(info_t *info, char *str)
{
	char *p;

	p = strchar(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (alias_unset(info, str));

	alias_unset(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints alias string
 * @node: Alias node
 * Return: Always 0 on success, 1 on error
 */

int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = strchar(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * alias - Mimics  alias builtin.
 * @info: Structure containing potential arguments.
 *  Return: Always 0
 */

int alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = strchar(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
