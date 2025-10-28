#include "../includes/minishell.h"

int	is_token_cmd(t_token *T)
{
	return (T->type == COMMAND || T->type == CMD_ARG || T->type == SINGL_QU
		|| T->type == DOUBLE_QU || T->type == QUOTED_VAR || T->type == VAR);
}

int	is_token_redirect(t_token *R)
{
	return (R->type == REDIR_IN || R->type == REDIR_OUT || R->type == APPEND
		|| R->type == HERE_DOC);
}

int is_it_opp(t_token *op)
{
	return(op->type == AND || op->type == OR || op->type == PIPE);
}

int is_empty_string(t_token *token)
{
	int len;
	
	len = ft_strlen(token->value);
	if(len == 0)
		return(1);
	if (token->type == DOUBLE_QU && len == 2 && 
		token->value[0] == '"' && token->value[1] == '"')
		return (1);
	if (token->type == SINGL_QU && len == 2 && 
		token->value[0] == '\'' && token->value[1] == '\'')
		return (1);
    return (0);
}

int red_flag(t_token *token)
{
    return (is_token_redirect(token));
}