#include "../includes/minishell.h"

t_token	*create_token(NodeType type, char *value, char *var_nam)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->var_nam = var_nam;
	new_token->next = NULL;
	return (new_token);
}

void clear_tokens(t_token **head)
{
    t_token *tmp;

    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp->value);
        free(tmp->var_nam);
        free(tmp);
    }
}

void print_tokens(t_token *tokens)
{
    while (tokens) {
        printf("Token: %s (type: %d)\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

void append_list(t_token **head, t_token *new_node)
{
	if (!*head)
		*head = new_node;
	else
	{
		t_token *temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

void free_token_list(t_token *token)
{
	t_token *tmp;
	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token->var_nam);
		free(token);
		token = tmp;
	}
}

int token_has_quotes(t_token *token)
{
    int i = 0;
    while (token->value[i])
    {
        if (token->value[i] == '\'' || token->value[i] == '"')
            return 1;
        i++;
    }
    return 0;
}

void free_split(char **strs)
{
    int i;

    if (!strs)
        return;
    i = 0;
    while(strs[i])
    {
        free(strs[i]);
        i++;
    }
    free(strs);
}