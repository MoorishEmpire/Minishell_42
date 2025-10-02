#include "../includes/minishell.h"

void	handle_fork_error(char *path, char **envp)
{
	ft_putstr_fd("minishell: fork failed\n", 2);
	g_exit_status = 1;
	free(path);
	free_2d_arr(envp);
}

void	handle_path_not_found(t_cmd *cmd)
{
	if (access(cmd->argv[0], F_OK) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": Permission denied", 2);
		ft_putstr_fd("\n", 2);
		g_exit_status = 126;
		return ;
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found", 2);
		ft_putstr_fd("\n", 2);
		g_exit_status = 127;
	}
}
