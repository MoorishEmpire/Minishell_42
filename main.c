#include "includes/minishell.h"

int g_exit_status;

static void	execute_command(t_cmd *cmd, t_env **env_list)
{
	if (!cmd)
		return ;
	if (collecting_heredoc(cmd, *env_list))
	{
		reset_heredoc_state();
		return ;
	}
	if (cmd && cmd->next)
		execute_pipeline(cmd, *env_list);
	else if (cmd)
	{
		if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
			execute_builtin(cmd, env_list);
		else if (cmd->argv && cmd->argv[0])
			exec_externals(cmd, *env_list);
	}
}

static void	process_line(char *input, char **envp, t_env **env_list)
{
	t_token	*head;
	t_token	*tail;
	t_token	*output;
	t_token	*expand;
	t_token	*wild;
	t_token	*strip;
	t_cmd	*cmd;

	head = NULL;
	tail = NULL;
	reset_heredoc_state();
	output = tokenizer(&head, &tail, input);
	if (!output)
		return ;
	expand = expanding_it(output, envp);
	clear_tokens(&output);
	if (!expand)
		return ;
	wild = handel_wild_card(expand);
	clear_tokens(&expand);
	if (!wild)
		return ;
	strip = stripper(wild);
	clear_tokens(&wild);
	if (!strip)
		return ;
	cmd = build_cmd_list(strip);
	clear_tokens(&strip);
	if (cmd)
	{
		execute_command(cmd, env_list);
		clear_cmd(&cmd);
	}
}

static void	exit_fun(t_env *env_list)
{
	write(1, "exit\n", 6);
	free_env(env_list);
	clear_history();
	exit(g_exit_status);
}

static int	check_unclosed_quotes(const char *line)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

static char	*read_complete_line(void)
{
	char	*line;
	char	*more;
	char	*tmp;
	char	*joined;

	line = readline("\033[1;96m minishell > \033[0m");
	if (!line)
		return (NULL);
	if (*line)
		add_history(line);
	while (check_unclosed_quotes(line))
	{
		more = readline("> ");
		if (!more)
		{
			ft_putstr_fd("bash: unexpected EOF while looking for matching quote\n", 2);
			ft_putstr_fd("bash: syntax error: unexpected end of file\n", 2);
			free(line);
			return (NULL);
		}
		tmp = ft_strjoin(line, "\n");
		joined = ft_strjoin(tmp, more);
		free(line);
		free(tmp);
		free(more);
		line = joined;
	}
	return (line);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*env_list;

	(void)ac;
	(void)av;
	g_exit_status = 0;
	env_list = env_to_list(env);
	set_signals_interactive();
	while (1)
	{
		input = read_complete_line();
		if (!input)
			exit_fun(env_list);
		if (*input)
			process_line(input, env, &env_list);
		free(input);
	}
	clear_history();
	free_env(env_list);
	return (g_exit_status);
}
