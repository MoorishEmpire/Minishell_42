#include "../../includes/minishell.h"

static void	handle_heredoc_child(int pipe_fd[2], char *delimiter, t_env *env,
		int expand)
{
	char	*line;

	set_signals_heredoc();
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		write_line_to_pipe(pipe_fd[1], line, env, expand);
		free(line);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_heredoc_parent(int pipe_fd[2], t_file *file, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		close(pipe_fd[0]);
		g_exit_status = 1;
		file->h_fd = -1;
	}
	else
		file->h_fd = pipe_fd[0];
	set_signals_interactive();
}

static int	setup_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", 2);
		return (0);
	}
	return (1);
}

static int	process_heredoc(t_file *file, t_env *env)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		expand;

	if (!setup_heredoc_pipe(pipe_fd))
		return (1);
	expand = !file->quoted;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (pid == 0)
		handle_heredoc_child(pipe_fd, file->name, env, expand);
	else
	{
		handle_heredoc_parent(pipe_fd, file, pid);
		if (g_exit_status == 1)
			return (1);
	}
	return (0);
}

int	collecting_heredoc(t_cmd *cmd, t_env *env)
{
	t_cmd	*cur;
	t_file	*file;

	cur = cmd;
	g_exit_status = 0;
	while (cur)
	{
		file = cur->file;
		while (file)
		{
			if (file->type == TOKEN_HEREDOC
				|| file->type == TOKEN_HEREDOC_quoted)
			{
				if (process_heredoc(file, env))
					return (1);
			}
			file = file->next;
		}
		cur = cur->next;
	}
	return (0);
}