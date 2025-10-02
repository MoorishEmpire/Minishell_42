#include "../includes/minishell.h"

int ft_isspace(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == ' ' || str[i] == '\t')
            return(1);
        i++;
    }
    return(0);
}

int redir_check(t_token *token)
{
    if (!token)
    {
        printf("bash: syntax error near unexpected token `newline'\n");
        g_exit_status = 2;
        return(1);
    }
    if (is_token_redirect(token) || is_it_opp(token))
    {
        printf("syntax error near unexpected token `%s'\n", token->value);
        g_exit_status = 2;
        return(1);
    }
    
    if ((token->type == VAR || token->type == QUOTED_VAR) &&
        is_empty_string(token))
    {
        printf("bash: $%s: ambiguous redirect\n",token->var_nam);
        g_exit_status = 1;
        return(1);
    }
    if ((token->type == VAR || token->type == QUOTED_VAR) &&
        (ft_isspace(token->value) || (token->var_nam != NULL && token->next 
            &&token->var_nam == token->next->var_nam)))
    {
        printf("bash: $%s: ambiguous redirect\n",token->var_nam);
        g_exit_status = 1;
        return(1);
    }
	return(0);
}