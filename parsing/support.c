#include "../includes/minishell.h"

int is_it_var(char *str)
{
	if(ft_strchr(str,'$'))
		return (1);
	else 
		return (0);
}

int is_it_singled(t_token *token)
{
    int in_single = 0;
    int in_double = 0;
    int i = 0;

    if (token->type == DOUBLE_QU) {
        return 0;
    }

    while (token->value[i]) {
        if (token->value[i] == '\'' && !in_double) {
            in_single = !in_single;
        }
        else if (token->value[i] == '"' && !in_single) {
            in_double = !in_double;
        }
        else if (token->value[i] == '$' && in_single) {
            return 1;
        }
        i++;
    }
    return 0;
}

int is_it_doubled(t_token *token)
{
    int in_single = 0;
    int in_double = 0;
    int i = 0;

    if (token->type == SINGL_QU) {
        return 0;
    }

    while (token->value[i]) {
        if (token->value[i] == '"' && !in_single) {
            in_double = !in_double;
        }
        else if (token->value[i] == '\'' && !in_double) {
            in_single = !in_single;
        }
        else if (token->value[i] == '$' && in_double) {
            return 1;
        }
        i++;
    }
    return 0;
}

char *var_name(char *str)
{
    char *dollar;
    char *tmp;

	int (i), (j), (start);
	j = 0;
    tmp = ft_strchr(str, '$');
    if (!tmp)
        return NULL;
    start = 1;
    i = 0;
    while (tmp[start + i] && (ft_isalnum(tmp[start + i]) || tmp[start + i] == '_'))
        i++;
    dollar = malloc(sizeof(char) * (i + 1));
    if (!dollar)
        return NULL;
    while ( j < i)
	{
		dollar[j] = tmp[start + j];
		j++;
	}
    dollar[i] = '\0';
    return dollar;
}