#include "../includes/minishell.h"

void	write_line_to_pipe(int pipe_fd, char *line, t_env *env, int expand)
{
	char	*expanded_line;
	t_token	*tmp;

	tmp = NULL;
	if (!line)
		return ;
	if (expand && ft_strchr(line, '$'))
	{
		expanded_line = expand_env(1, line, env, &tmp);
		if (expanded_line)
		{
			write(pipe_fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(pipe_fd, line, ft_strlen(line));
	}
	else
		write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
}