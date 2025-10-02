#include "../includes/minishell.h"

void	reset_heredoc_state(void)
{
	set_signals_interactive();
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 2);
	close(STDIN_FILENO);
	exit(1);
}

void	set_signals_heredoc(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	update_exit_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}