#include "../includes/minishell.h"

static t_token	*create_stripped_token(t_token *token)
{
	char	*stripped;
	t_token	*new;

	stripped = strip_token(token->value);
	if (!stripped)
		return (NULL);
	new = create_token(token->type, stripped, token->var_nam);
	free(stripped);
	return (new);
}

t_token	*stripper(t_token *xpnd)
{
	t_token	*result;
	t_token	*new;

	result = NULL;
	while (xpnd)
	{
		new = create_stripped_token(xpnd);
		if (!new)
		{
			clear_tokens(&result);
			return (NULL);
		}
		append_list(&result, new);
		xpnd = xpnd->next;
	}
	return (result);
}
