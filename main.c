#include "includes/minishell.h"

int g_exit_status = 0;

static void execute_command(t_cmd *cmd, t_env **env_list)
{
    if (!cmd)
        return;
    
    // Process heredocs first
    if (collecting_heredoc(cmd, *env_list))
    {
        reset_heredoc_state();
        return;
    }
    
    // Check if it's a pipeline (multiple commands)
    if (cmd && cmd->next)
    {
        execute_pipeline(cmd, *env_list);
    }
    else if (cmd)
    {
        // Single command execution
        if (cmd->argv && cmd->argv[0] && is_builtin(cmd->argv[0]))
            execute_builtin(cmd, env_list);
        else if (cmd->argv && cmd->argv[0])
            exec_externals(cmd, *env_list);
        else if (cmd->file)
        {
            // Handle redirections only (no command)
            apply_redirection(cmd);
        }
    }
}

static void process_line(char *input, t_env **env_list)
{
    t_token *head   = NULL;
    t_token *tail   = NULL;
    t_token *output = NULL;
    t_token *expand = NULL;
    t_token *wild   = NULL;
    t_token *strip  = NULL;
    t_cmd   *cmd    = NULL;

    // Parsing pipeline
    output = tokenizer(&head, &tail, input);
    if (!output)
        return;
    
    // Check for syntax errors
    if (pipe_err(&output) || redir_check(output))
    {
        clear_tokens(&output);
        g_exit_status = 2;
        return;
    }
    
    // Expansion pipeline
    expand = expanding_it(output, list_to_env(*env_list));
    wild   = handel_wild_card(expand);
    strip  = stripper(wild);
    
    // Build command structure
    cmd = build_cmd_list(strip);
    
    // Execute the command
    if (cmd)
    {
        execute_command(cmd, env_list);
        clear_cmd(&cmd);
    }
    
    // Cleanup
    clear_tokens(&output);
    clear_tokens(&expand);
    clear_tokens(&wild);
    clear_tokens(&strip);
}

void clear_cmd(t_cmd **head)
{
    t_cmd *tmp;
    int i;

    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        if (tmp->argv)
        {
            i = 0;
            while (tmp->argv[i])
                free(tmp->argv[i++]);
            free(tmp->argv);
        }
        if (tmp->redirect)
        {
            i = 0;
            while (tmp->redirect[i])
                free(tmp->redirect[i++]);
            free(tmp->redirect);
        }
        if (tmp->file)
        {
            i = 0;
            while (tmp->file[i])
                free(tmp->file[i++]);
            free(tmp->file);
        }
        free(tmp);
    }
}

static int check_unclosed_quotes(const char *line)
{
    int in_single = 0;
    int in_double = 0;
    int i = 0;

    while (line[i])
    {
        if (line[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (line[i] == '"' && !in_single)
            in_double = !in_double;
        i++;
    }
    return (in_single || in_double);
}

static char *read_complete_line(void)
{
    char *line;
    char *more;
    char *tmp;
    char *joined;
    
    line = readline("\033[1;96mminishell$ \033[0m");
    if (!line)
    {
        ft_putstr_fd("exit\n", 1);
        return (NULL);
    }
    
    if (*line)
        add_history(line);
    
    while (check_unclosed_quotes(line))
    {
        more = readline("> ");
        if (!more)
        {
            ft_putstr_fd("minishell: unexpected EOF while looking for matching quote\n", 2);
            ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
            free(line);
            g_exit_status = 2;
            return (NULL);
        }
        tmp = ft_strjoin(line, "\n");
        joined = ft_strjoin(tmp, more);
        free(line);
        free(tmp);
        free(more);
        line = joined;
    }
    return (line);
}

static void exit_shell(int status)
{
    ft_putstr_fd("exit\n", 1);
    clear_history();
    exit(status);
}

int main(int argc, char **argv, char **env)
{
    char    *line;
    t_env   *env_list;
    
    (void)argc;
    (void)argv;
    
    // Initialize environment
    env_list = env_to_list(env);
    if (!env_list)
    {
        ft_putstr_fd("minishell: failed to initialize environment\n", 2);
        return (1);
    }
    
    // Set up signal handlers
    set_signals_interactive();
    
    // Main shell loop
    while (1)
    {
        line = read_complete_line();
        if (!line)
            exit_shell(g_exit_status);
        
        if (*line)
            process_line(line, &env_list);
        
        free(line);
    }
    
    // Cleanup (unreachable in normal execution)
    clear_history();
    free_env(env_list);
    return (g_exit_status);
}

/* Helper function to free environment list */
void free_env(t_env *env)
{
    t_env *tmp;
    
    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->var);
        free(tmp->value);
        free(tmp);
    }
}