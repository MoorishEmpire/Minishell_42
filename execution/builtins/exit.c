#include "../execution.h"

static int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	handel_args(char **args)
{
	long long	val;
	int			i;

	if (!is_num(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);
	}
	else if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (-1);
	}
	/* parse value within long long range, fallback to modulo behavior */
	val = 0;
	i = (args[1][0] == '+' || args[1][0] == '-') ? 1 : 0;
	while (args[1][i])
	{
		val = val * 10 + (args[1][i] - '0');
		i++;
	}
	if (args[1][0] == '-')
		val = -val;
	return ((int)(val % 256));
}

void		ft_exit(char **args)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (args[1])
	{
		exit_code = handel_args(args);
		if (exit_code == -1)
			return ;
	}
	else
		exit_code = g_exit_status;
	clear_history();
	exit(exit_code);
}
