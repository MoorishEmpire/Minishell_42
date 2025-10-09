#include "../includes/minishell.h"

/* Convert parsing NodeType to execution t_token_type for redirections */
/*
static t_token_type convert_redir_type(NodeType type)
{
    if (type == REDIR_IN)
        return (TOKEN_REDIR_IN);
    else if (type == REDIR_OUT)
        return (TOKEN_REDIR_OUT);
    else if (type == APPEND)
        return (TOKEN_APPEND);
    else if (type == HERE_DOC)
        return (TOKEN_HEREDOC);
    return (TOKEN_WORD);
}
*/

/* Build t_file linked list from redirect and file arrays */
t_file *build_file_list(char **redirect, char **file)
{
    t_file  *head;
    t_file  *current;
    t_file  *new;
    int     i;

    if (!redirect || !file)
        return (NULL);
    
    head = NULL;
    current = NULL;
    i = 0;
    
    while (redirect[i] && file[i])
    {
        new = malloc(sizeof(t_file));
        if (!new)
            return (head);
        
        new->name = ft_strdup(file[i]);
        
        /* Determine the redirection type */
        if (ft_strcmp(redirect[i], "<") == 0)
            new->type = TOKEN_REDIR_IN;
        else if (ft_strcmp(redirect[i], ">") == 0)
            new->type = TOKEN_REDIR_OUT;
        else if (ft_strcmp(redirect[i], ">>") == 0)
            new->type = TOKEN_APPEND;
        else if (ft_strcmp(redirect[i], "<<") == 0)
        {
            new->type = TOKEN_HEREDOC;
            new->quoted = 0;  /* You may need to determine this from parsing */
        }
        else
            new->type = TOKEN_WORD;
        
        new->h_fd = -1;
        new->next = NULL;
        
        if (!head)
        {
            head = new;
            current = new;
        }
        else
        {
            current->next = new;
            current = new;
        }
        i++;
    }
    
    return (head);
}

/* Free t_file linked list */
void free_file_list(t_file *file)
{
    t_file *tmp;
    
    while (file)
    {
        tmp = file;
        file = file->next;
        free(tmp->name);
        free(tmp);
    }
}

/* Update apply_redirection to handle both formats */
void apply_redirection_new(t_cmd *cmd)
{
    t_file *file_list;
    
    /* If cmd has t_file linked list (from execution), use it directly */
    if (cmd->file && cmd->redirect)
    {
        /* Build t_file linked list from arrays */
        file_list = build_file_list(cmd->redirect, cmd->file);
        if (file_list)
        {
            t_file *f = file_list;
            while (f)
            {
                handle_redir(f);
                f = f->next;
            }
            free_file_list(file_list);
        }
    }
}
