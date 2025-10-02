#include "../includes/minishell.h"

int	get_redir_type(char *redir_str)
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

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
