#include "../includes/minishell.h"

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

static void	handle_heredoc_parent(int pipe_fd[2], int *h_fd, pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status))
	{
		close(pipe_fd[0]);
		g_exit_status = 1;
		*h_fd = -1;
	}
	else
		*h_fd = pipe_fd[0];
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

static int	process_heredoc(char *delimiter, int quoted, t_env *env, int *h_fd)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		expand;

	if (!setup_heredoc_pipe(pipe_fd))
		return (1);
	expand = !quoted;
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd("minishell: fork error\n", 2);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (pid == 0)
		handle_heredoc_child(pipe_fd, delimiter, env, expand);
	else
	{
		handle_heredoc_parent(pipe_fd, h_fd, pid);
		if (g_exit_status == 1)
			return (1);
	}
	return (0);
}

static int	is_heredoc_operator(char *op)
{
	if (!op)
		return (0);
	return (ft_strcmp(op, "<<") == 0);
}

int	collecting_heredoc(t_cmd *cmd, t_env *env)
{
	t_cmd	*cur;
	int		i;
	int		h_fd;

	cur = cmd;
	g_exit_status = 0;
	while (cur)
	{
		if (!cur->redirect || !cur->file)
		{
			cur = cur->next;
			continue ;
		}
		i = 0;
		while (cur->redirect[i] && cur->file[i])
		{
			if (is_heredoc_operator(cur->redirect[i]))
			{
				if (process_heredoc(cur->file[i], 0, env, &h_fd))
					return (1);
				/* Store h_fd for later use - you may need to add this to t_cmd */
			}
			i++;
		}
		cur = cur->next;
	}
	return (0);
}
