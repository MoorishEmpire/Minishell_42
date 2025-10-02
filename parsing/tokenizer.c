#include "../includes/minishell.h"

void is_it_pipe(t_token **head, t_token **tail, int *i, char *input)
{
    t_token *new;

	new = NULL;
    if(input[*i] == '|' && input[*i+1] == '|' )
    {
        new = create_token(OR, "||",NULL);
        *i += 1;
    } 
    else if(input[*i] == '|')
        new = create_token(PIPE, "|",NULL);
    if (!new)
        return;
    if (!*head)
        *head = new;
    else
        (*tail)->next = new;
    *tail = new;
    (*i)++;
}

void get_full_quote(char *input, int* i, int *in_q, char *type, int *len)
{
	    while (input[*i])
    {
        if (!*in_q && is_token_breaker(input[*i]))
            break;
        if ((input[*i] == '"' || input[*i] == '\''))
        {
            if (!*in_q)
            {
                *in_q = 1;
               *type = input[*i];
            }
            else if (*type == input[*i])
                *in_q = 0;
        }
        (*i)++;
       (*len)++;
    }
}

int make_full_token(char *input, int len, int start, t_token **new)
{
	int j;
	char *tmp;

	j = 0;
	tmp = malloc(sizeof(char) * (len + 1));
    if (!tmp)
        return(-1);
    while (j < len)
	{
	    tmp[j] = input[start + j];
		j++;
	}
	tmp[len] = '\0';
    if(input[start] == '-')
        *new = create_token(CMD_ARG, tmp,NULL);
    else if (input[start] == '"')
        *new = create_token(DOUBLE_QU, tmp,NULL);
    else if (input[start] == '\'')
        *new = create_token(SINGL_QU, tmp,NULL);
    else
        *new = create_token(COMMAND, tmp,NULL);
    free(tmp);
	return(j);
}

t_token *tokenizer(t_token **head, t_token **tail,char *input)
{
    int i = 0;
    
    while (input[i])
        {
            if (input[i] == ' ')
            {
                i++;
                continue;
            }
            if (input[i] == '|') 
                is_it_pipe(head, tail, &i, input);
			else if(input[i] == '&')
				is_it_and(head, tail, &i, input);
            else if (input[i] == '<' || input[i] == '>')
                is_it_op(head, tail, &i, input);
            else
                is_it_word(head, tail, &i, input);
        }
        return(*head);
}