#ifndef MINISHELL_H
# define MINISHELL_H

/* System headers */
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Libft */
# include "../utils/libft/libft.h"

/* Color definitions */
# define REDD     "\033[0;101m"
# define WHITE    "\033[0;7m"
# define GREEN    "\033[0;42m"
# define RED      "\033[0;95m"
# define YELLOW   "\033[0;33m"
# define SKYBLUE  "\033[0;36m"
# define PURPLE   "\033[0;35m"
# define RESET    "\033[0m"

/* Exit codes */
# define CMD_NOT_FOUND 127

/* Global exit status */
extern int g_exit_status;

/* =============== PARSING STRUCTURES =============== */

typedef enum {
    COMMAND,        // 0
    CMD_ARG,        // 1
    OR,
    AND,
    SINGL_QU,       // 4
    DOUBLE_QU,      // 5
    PIPE,           // 6
    REDIR_IN,       // 7
    REDIR_OUT,      // 8
    APPEND,         // 9
    HERE_DOC,       // 10
    FILE_NAME,
    VAR,
    QUOTED_VAR,
} NodeType;

typedef struct s_token {
    char            *value;
    char            *var_nam;
    NodeType        type;
    int             quote_flag;
    struct s_token  *next;
} t_token;

typedef struct s_ctx {
    int i;
    int j;
    int in_single;
    int in_double;
    int len;
} t_ctx;

/* =============== EXECUTION STRUCTURES =============== */

/* Token types for execution (different from parsing NodeType) */
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_SPACE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_HEREDOC_quoted,
    TOKEN_PIPE
} t_token_type;

/* File structure for redirections */
typedef struct s_file {
    char            *name;
    t_token_type    type;
    int             h_fd;
    int             quoted;
    struct s_file   *next;
} t_file;

/* Command structure - shared between parsing and execution */
typedef struct s_cmd {
    char            **argv;
    char            **redirect;
    char            **file;
    struct s_cmd    *next;
} t_cmd;

/* Environment variable structure */
typedef struct s_env {
    char            *var;
    char            *value;
    struct s_env    *next;
} t_env;

/* Context structures for pipeline execution */
typedef struct s_child_ctx {
    t_cmd   *cur;
    t_env   *env;
    int     **pipes;
    int     i;
    int     cmd_count;
} t_child_ctx;

typedef struct s_pipes_ctx {
    int     **pipes;
    pid_t   *pids;
    int     cmd_count;
    t_cmd   *cur;
    int     i;
} t_pipes_ctx;

/* =============== PARSING FUNCTIONS =============== */

/* Tokenizer and parsing */
t_token     *tokenizer(t_token **head, t_token **tail, char *input);
t_token     *create_token(NodeType type, char *value, char *var_nam);
void        clear_tokens(t_token **head);
void        print_tokens(t_token *tokens);
t_cmd       *build_cmd_list(t_token *token);
void        clear_cmd(t_cmd **head);

/* Expansion functions */
t_token     *expanding_it(t_token *token, char **env);
t_token     *expand_variables(t_token *tokens, char **envp);
t_token     *handel_wild_card(t_token *xpnd);
t_token     *stripper(t_token *xpnd);
char        *strip_token(char *value);
char        *remove_quotes(char *value);

/* Error checking */
int         pipe_err(t_token **list);
int         redir_check(t_token *token);
int         is_token_redirect(t_token *R);
int         is_it_doubled(t_token *dollar);
int         is_it_singled(t_token *dollar);
int         is_token_cmd(t_token *T);
int         is_it_opp(t_token *op);
int         is_token_breaker(char c);
int         is_empty_string(t_token *token);
int         red_flag(t_token *token);
int         token_has_quotes(t_token *token);

/* Variable handling */
char        *replace_in_quotes(char *str, char **env);
char        *strip_str(char *str);
char        *handle_double(t_token *token, char **env);
char        *expand(t_token *tokens, char **env);
char        *replace_in_arg(char *str, char **env);
int         is_it_var(char *str);
char        *var_name(char *str);
int         double_dollars(char *result, int j);
int         handle_token(t_token *xpnd, t_token **result, t_token *prev);
int         split_var_token(t_token *xpnd, t_token **result);
int         copy_token(t_token *xpnd, t_token **result);

/* Tokenizer helpers */
void        is_it_and(t_token **head, t_token **tail, int *i, char *input);
void        is_it_pipe(t_token **head, t_token **tail, int *i, char *input);
void        is_it_op(t_token **head, t_token **tail, int *i, char *input);
void        is_it_quote(t_token **head, t_token **tail, int *i, char *input, char c);
void        is_it_word(t_token **head, t_token **tail, int *i, char *input);
int         make_full_token(char *input, int len, int start, t_token **new);
void        get_full_quote(char *input, int* i, int *in_q, char *type, int *len);
void        append_list(t_token **head, t_token *new_node);

/* Error printing */
void        print_system_error(char *context);
void        print_command_error(char *cmd, char *error_msg);
void        print_syntax_error(char *token);
void        print_file_error(char *filename);
void        print_ambiguous_redirect(char *var_name);
void        print_cmd_not_found(char *cmd);

/* =============== EXECUTION FUNCTIONS =============== */

/* Builtin functions */
void        ft_echo(char **args);
int         ft_cd(char **args, t_env **env);
int         ft_pwd(void);
int         ft_env(t_env *env);
void        ft_exit(char **args);
int         ft_export(char **args, t_env **env);
int         ft_unset(char **args, t_env **env);
int         execute_builtin(t_cmd *cmd, t_env **env);
int         is_builtin(char *cmd);
int         is_valid_key(char *key);

/* Environment functions */
t_env       *env_to_list(char **env);
char        **list_to_env(t_env *list);
char        *get_env_value(t_env *env, const char *key);
void        update_env_var(t_env **env, const char *key, const char *value);
void        free_env(t_env *env);

/* Execution functions */
void        execute_pipeline(t_cmd *cmds, t_env *env);
void        exec_externals(t_cmd *cmd, t_env *env);
void        exec_externals_in_child(t_cmd *cmd, t_env *env);
void        child_proc(t_child_ctx *ctx);
char        *get_cmd_path(char *cmd, t_env *env);
void        execute_child_process(t_cmd *cmd, char *path, char **envp);
void        apply_redirection(t_cmd *cmd);
void        handle_redir(t_file *f);
void        handle_heredoc(t_file *f);
int         open_file_by_type(t_file *f);

/* Pipeline helpers */
int         initialize_pipeline_resources(t_pipes_ctx *p_ctx, t_cmd *cmds);
void        execute_pipeline_commands(t_pipes_ctx *p_ctx, t_env *env, t_cmd *cmds);
void        finalize_pipeline_execution(t_pipes_ctx *p_ctx);
void        cleanup_pipeline_resources(t_pipes_ctx *p_ctx);
void        close_pipes(int **pipe, int count);
void        free_pipes(int **pipes, int count);
void        wait_all(pid_t *pids, int count);

/* Heredoc functions */
int         collecting_heredoc(t_cmd *cmd, t_env *env);
void        write_line_to_pipe(int pipe_fd, char *line, t_env *env, int expand);
char        *expand_env(int flag, char *str, t_env *env, t_token **tmp);

/* Signal handling */
void        set_signals_interactive(void);
void        set_signals_in_child(void);
void        set_signals_for_child_execution(void);
void        set_signals_heredoc(void);
void        handle_sigint(int sig);
void        handle_sigint_heredoc(int sig);
void        reset_heredoc_state(void);
void        update_exit_status(int status);

/* Error handling and utilities */
void        handle_fork_error(char *path, char **envp);
void        handle_path_not_found(t_cmd *cmd);
int         useless(t_cmd *cmd, char **path);
int         error_fun(void);
void        malloc_error(void);
int         count_cmd(t_cmd *cmd);

/* Wildcard expansion */
void        expand_wildcard(t_token **tokens, t_token **last, t_token *current);
void        add_token(t_token **tokens, t_token **last, t_token_type type, char *value);

/* Redirection conversion helpers */
t_file      *build_file_list(char **redirect, char **file);
void        free_file_list(t_file *file);

/* Additional parsing helpers */
char        *get_env_value_array(char *name, char **env);

/* Libft functions */
int         ft_strcmp(const char *s1, const char *s2);
int         ft_strncmp(const char *s1, const char *s2, size_t n);
char        *ft_strdup(const char *s);
char        *ft_strjoin(const char *s1, const char *s2);
char        *ft_substr(const char *s, unsigned int start, size_t len);
char        *ft_strchr(const char *s, int c);
void        ft_putstr_fd(const char *s, int fd);
int         ft_isdigit(int c);
int         ft_isalpha(int c);
int         ft_isalnum(int c);
size_t      ft_strlen(const char *s);
char        **ft_split(const char *s, char c);
char        *ft_itoa(int n);
void        free_2d_arr(char **arr);
void        free_split(char **strs);

#endif /* MINISHELL_H */