#include "../includes/minishell.h"

void skip_whitespace(int *i, char *input)
{
    while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
        (*i)++;
}

void is_it_and(t_token **head, t_token **tail, int *i, char *input)
{
	t_token *new;
	new = NULL;
    if(input[*i] == '&' && input[*i+1] == '&' )
    {
        new = create_token(AND, "&&",NULL);
        *i += 1;
    }
	if (!new)
        return;
    if (!*head)
        *head = new;
    else
        (*tail)->next = new;
    *tail = new;
    (*i)++;
}

int is_token_breaker(char c) {
    return (c == ' ' || c == '|'||
            c == '<' || c == '>');
}

void is_it_word(t_token **head, t_token **tail, int *i, char *input)
{
    t_token *new;
	char quote_type;
	
    int (len), (start), (j),(in_quote);
    len = 0;
    j = 0;
    start = *i;
    in_quote = 0;
    quote_type = 0;
	new = NULL;
	get_full_quote(input,i,&in_quote,&quote_type,&len);
    if (len == 0)
        return; 
	j = make_full_token(input,len,start,&new);
	if(j == -1)
		return;
    if (!new)
        return;
    if (!*head)
        *head = new;
    else
        (*tail)->next = new;
    *tail = new;
}

void is_it_op(t_token **head, t_token **tail, int *i, char *input)
{
    t_token *new;
    
	new = NULL;
    if(input[*i] == '<' && input[*i+1] == '<' )
    {
        new = create_token(HERE_DOC, "<<",NULL);
        *i += 1;
    }  
    else if(input[*i] == '>' && input[*i+1] == '>')
    {
        new = create_token(APPEND, ">>",NULL);
        *i += 1;
    }
    else if(input[*i] == '>')
        new = create_token(REDIR_OUT, ">",NULL);
    else if(input[*i] == '<')
        new = create_token(REDIR_IN, "<",NULL);
    if (!new)
        exit(1);
    if (!*head)
        *head = new;
    else
        (*tail)->next = new;
    *tail = new;
    (*i)++;
}