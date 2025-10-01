#include "../../includes/minishell.h"

void	handle_heredoc(t_file *f)
{
	if (f->h_fd != -1)
	{
		dup2(f->h_fd, STDIN_FILENO);
		close(f->h_fd);
		f->h_fd = -1;
	}
}

int	open_file_by_type(t_file *f)
{
	int	fd;

	if (f->type == TOKEN_REDIR_IN)
		fd = open(f->name, O_RDONLY);
	else if (f->type == TOKEN_REDIR_OUT)
		fd = open(f->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (f->type == TOKEN_APPEND)
		fd = open(f->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = -1;
	return (fd);
}

void	apply_redirection(t_cmd *cmd)
{
	t_file	*f;

	f = cmd->file;
	while (f)
	{
		handle_redir(f);
		f = f->next;
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

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
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