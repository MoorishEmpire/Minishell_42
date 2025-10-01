#include "../execution.h"

static void	run_builtin(t_cmd *cmd, t_env **env)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	name = cmd->argv[0];
	if (ft_strcmp(name, "echo") == 0)
		ft_echo(cmd->argv);
	else if (ft_strcmp(name, "cd") == 0)
		g_exit_status = ft_cd(cmd->argv, env);
	else if (ft_strcmp(name, "pwd") == 0)
		g_exit_status = ft_pwd();
	else if (ft_strcmp(name, "env") == 0)
		g_exit_status = ft_env(*env);
	else if (ft_strcmp(name, "exit") == 0)
		ft_exit(cmd->argv);
	else if (ft_strcmp(name, "export") == 0)
		g_exit_status = ft_export(cmd->argv, env);
	else if (ft_strcmp(name, "unset") == 0)
		g_exit_status = ft_unset(cmd->argv, env);
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = CMD_NOT_FOUND;
	}
}

int		execute_builtin(t_cmd *cmd, t_env **env)
{
	int	stdin_copy;
	int	stdout_copy;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	stdin_copy = dup(STDIN_FILENO);
	if (stdin_copy == -1)
		return (1);
	stdout_copy = dup(STDOUT_FILENO);
	if (stdout_copy == -1)
	{
		close(stdin_copy);
		return (1);
	}
	apply_redirection(cmd);
	run_builtin(cmd, env);
	if (dup2(stdin_copy, STDIN_FILENO) == -1)
		; 
	if (dup2(stdout_copy, STDOUT_FILENO) == -1)
		;
	close(stdin_copy);
	close(stdout_copy);
	return (g_exit_status);
}
