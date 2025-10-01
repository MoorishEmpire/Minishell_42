#include "../../includes/minishell.h"

int	useless(t_cmd *cmd, char **path)
{
	if (!path)
	{
		handle_path_not_found(cmd);
		return (0);
	}
	return (1);
}

int	error_fun(void)
{
	ft_putstr_fd("Error: Unclosed quotes!\n", 2);
	g_exit_status = 258;
	return (0);
}

void	malloc_error(void)
{
	ft_putstr_fd("malloc Error !\n", 2);
	exit(EXIT_FAILURE);
}

int	count_cmd(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}
