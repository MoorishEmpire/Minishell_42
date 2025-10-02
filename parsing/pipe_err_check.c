#include "../includes/minishell.h"

int pipe_err(t_token **list)
{
	t_token *curr = *list;
	t_token *last = NULL;

	if (curr && curr->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (1);
	}
	while (curr)
	{
		if (curr->type == PIPE && curr->next && curr->next->type == PIPE)
		{
			printf("syntax error near unexpected token `|'\n");
			g_exit_status = 2;
			return (1);
		}
		last = curr;
		curr = curr->next;
	}
	if (last && last->type == PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		g_exit_status = 2;
		return (1);
	}
	return (0);
}