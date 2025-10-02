#include "../includes/minishell.h"

int	arg_count(t_token *token, NodeType i)
{
	int	j;

	j = 0;
	while (token && token->type != i)
	{
		j++;
		token = token->next;
	}
	return (j);
}

int	redir_counter(t_token *token, NodeType i)
{
	int	j;

	j = 0;
	while (token && token->type != i)
	{
		if (is_token_redirect(token))
			j++;
		token = token->next;
	}
	return (j);
}

t_cmd	*populate_cmd_data(t_cmd *cmd, t_token *token)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (token && token->type != PIPE)
	{
		if (is_token_cmd(token))
			cmd->argv[i++] = ft_strdup(token->value);
		else if (is_token_redirect(token))
		{
			cmd->redirect[j] = ft_strdup(token->value);
			token = token->next;
			if (redir_check(token))
				return (NULL);
			cmd->file[j] = ft_strdup(token->value);
			j++;
		}
		token = token->next;
	}
	cmd->argv[i] = NULL;
	cmd->redirect[j] = NULL;
	cmd->file[j] = NULL;
	return (cmd);
}

t_cmd	*store_cmds(t_token *token)
{
	t_cmd	*cmd;
	int		redir_count;

	redir_count = redir_counter(token, PIPE);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = malloc(sizeof(char *) * (arg_count(token, PIPE) + 1));
	cmd->redirect = malloc(sizeof(char *) * (redir_count + 1));
	cmd->file = malloc(sizeof(char *) * (redir_count + 1));
	if (!cmd->argv || !cmd->redirect || !cmd->file)
		return (NULL);
	cmd->next = NULL;
	if (!populate_cmd_data(cmd, token))
		return (NULL);
	return (cmd);
}

t_cmd	*build_cmd_list(t_token *token)
{
	t_cmd *head;
	t_cmd *tail;
	t_cmd *cmd;

	head = NULL;
	cmd = NULL;
	tail = NULL;
	if (pipe_err(&token))
		return NULL;
	while (token)
	{
		cmd = store_cmds(token);
		if (!head)
			head = cmd;
		else
			tail->next = cmd;
		tail = cmd;
		while (token && token->type != PIPE)
			token = token->next;
		if (token && token->type == PIPE)
			token = token->next;
	}
	return (head);
}