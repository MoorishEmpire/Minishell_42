#include "../includes/minishell.h"

/* Main stripper function to remove quotes from tokens */
t_token *stripper(t_token *wild)
{
    t_token *result = NULL;
    t_token *new;
    char *stripped_value;

    while (wild)
    {
        /* Strip quotes from token value based on type */
        if (wild->type == SINGL_QU || wild->type == DOUBLE_QU)
        {
            stripped_value = remove_quotes(wild->value);
            new = create_token(COMMAND, stripped_value, wild->var_nam);
            free(stripped_value);
        }
        else if (wild->type == QUOTED_VAR)
        {
            stripped_value = strip_token(wild->value);
            new = create_token(VAR, stripped_value, wild->var_nam);
            free(stripped_value);
        }
        else
        {
            /* Keep token as is */
            new = create_token(wild->type, wild->value, wild->var_nam);
        }
        
        if (!new)
        {
            free_token_list(result);
            return NULL;
        }
        
        append_list(&result, new);
        wild = wild->next;
    }
    
    return result;
}