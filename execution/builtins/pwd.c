#include "../execution.h"

int			ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory: ", 2);
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}
