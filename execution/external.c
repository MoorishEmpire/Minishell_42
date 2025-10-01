#include "../../includes/minishell.h"

static int	get_redir_type(char *redir_str)
{
	if (!redir_str)
		return (-1);
	if (ft_strcmp(redir_str, "<") == 0)
		return (TOKEN_REDIR_IN);
	else if (ft_strcmp(redir_str, ">") == 0)
		return (TOKEN_REDIR_OUT);
	else if (ft_strcmp(redir_str, ">>") == 0)
		return (TOKEN_APPEND);
	else if (ft_strcmp(redir_str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (-1);
}

static void	handle_redir_by_type(int type, char *filename)
{
	int	fd;

	if (type == TOKEN_REDIR_IN)
	{
		fd = open(filename, O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (type == TOKEN_REDIR_OUT)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (type == TOKEN_APPEND)
	{
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(filename, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	apply_redirection(t_cmd *cmd)
{
	int	i;
	int	type;

	if (!cmd || !cmd->redirect || !cmd->file)
		return ;
	i = 0;
	while (cmd->redirect[i] && cmd->file[i])
	{
		type = get_redir_type(cmd->redirect[i]);
		if (type != -1 && type != TOKEN_HEREDOC)
			handle_redir_by_type(type, cmd->file[i]);
		i++;
	}
}

void	execute_child_process(t_cmd *cmd, char *path, char **envp)
{
	set_signals_in_child();
	apply_redirection(cmd);
	execve(path, cmd->argv, envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found", 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

void	exec_externals(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	char	*path;
	char	**envp;
	int		status;

	path = get_cmd_path(cmd->argv[0], env);
	if (useless(cmd, &path) == 0)
		return ;
	envp = list_to_env(env);
	set_signals_for_child_execution();
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, path, envp);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		set_signals_interactive();
		update_exit_status(status);
	}
	else
		handle_fork_error(path, envp);
	free(path);
	free_2d_arr(envp);
}
