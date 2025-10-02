#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define CMD_NOT_FOUND 127

/* Token types for redirection */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SPACE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_HEREDOC_quoted,
	TOKEN_PIPE
}	t_token_type;

/* NodeType from parsing.h - keep for compatibility */
typedef enum {
	COMMAND,
	CMD_ARG,
	OR,
	AND,
	SINGL_QU,
	DOUBLE_QU,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HERE_DOC,
	FILE_NAME,
	VAR,
	QUOTED_VAR,
} NodeType;

/* File structure for redirections */
typedef struct s_file
{
	char			*name;
	t_token_type	type;
	int				h_fd;
	int				quoted;
	struct s_file	*next;
}	t_file;

/* Command structure */
typedef struct s_cmd
{
	char			**argv;
	char			**redirect;
	char			**file;
	struct s_cmd	*next;
}	t_cmd;

/* Environment variable structure */
typedef struct s_env
{
	char			*var;
	char			*value;
	struct s_env	*next;
}	t_env;

/* Token structure for parsing */
typedef struct s_token
{
	NodeType		type;
	char			*value;
	char			*var_nam;
	int				quote_flag;
	struct s_token	*next;
}	t_token;

/* Context structures for pipeline execution */
typedef struct s_child_ctx
{
	t_cmd	*cur;
	t_env	*env;
	int		**pipes;
	int		i;
	int		cmd_count;
}	t_child_ctx;

typedef struct s_pipes_ctx
{
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	t_cmd	*cur;
	int		i;
}	t_pipes_ctx;

/* Global exit status */
extern int	g_exit_status;

/* Builtin functions */
void	ft_echo(char **args);
int		ft_cd(char **args, t_env **env);
int		ft_pwd(void);
int		ft_env(t_env *env);
void	ft_exit(char **args);
int		ft_export(char **args, t_env **env);
int		ft_unset(char **args, t_env **env);
int		execute_builtin(t_cmd *cmd, t_env **env);
int		is_builtin(char *cmd);
int		is_valid_key(char *key);

/* Environment functions */
t_env	*env_to_list(char **env);
char	**list_to_env(t_env *list);
char	*get_env_value(t_env *env, const char *key);
void	update_env_var(t_env **env, const char *key, const char *value);

/* Execution functions */
void	execute_pipeline(t_cmd *cmds, t_env *env);
void	exec_externals(t_cmd *cmd, t_env *env);
void	exec_externals_in_child(t_cmd *cmd, t_env *env);
void	child_proc(t_child_ctx *ctx);
char	*get_cmd_path(char *cmd, t_env *env);
void	execute_child_process(t_cmd *cmd, char *path, char **envp);
void	apply_redirection(t_cmd *cmd);

/* Pipeline helpers */
int		initialize_pipeline_resources(t_pipes_ctx *p_ctx, t_cmd *cmds);
void	execute_pipeline_commands(t_pipes_ctx *p_ctx, t_env *env, t_cmd *cmds);
void	finalize_pipeline_execution(t_pipes_ctx *p_ctx);
void	cleanup_pipeline_resources(t_pipes_ctx *p_ctx);
void	close_pipes(int **pipe, int count);
void	free_pipes(int **pipes, int count);
void	wait_all(pid_t *pids, int count);

/* Heredoc functions */
int		collecting_heredoc(t_cmd *cmd, t_env *env);
void	write_line_to_pipe(int pipe_fd, char *line, t_env *env, int expand);
char	*expand_env(int flag, char *str, t_env *env, t_token **tmp);

/* Signal handling */
void	set_signals_interactive(void);
void	set_signals_in_child(void);
void	set_signals_for_child_execution(void);
void	set_signals_heredoc(void);
void	handle_sigint(int sig);
void	handle_sigint_heredoc(int sig);
void	reset_heredoc_state(void);
void	update_exit_status(int status);

/* Error handling and utilities */
void	handle_fork_error(char *path, char **envp);
void	handle_path_not_found(t_cmd *cmd);
int		useless(t_cmd *cmd, char **path);
int		error_fun(void);
void	malloc_error(void);
int		count_cmd(t_cmd *cmd);
int		get_redir_type(char *redir_str);

/* Cleanup functions */
void	clear_tokens(t_token **head);
void	clear_cmd(t_cmd **head);
void	free_env(t_env *env);
void	free_2d_arr(char **arr);

/* Wildcard expansion */
void	expand_wildcard(t_token **tokens, t_token **last, t_token *current);
void	add_token(t_token **tokens, t_token **last, t_token_type type, char *value);

/* Libft string functions - declare here if not in libft header */
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *s, int c);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
void	ft_putstr_fd(char *s, int fd);

#endif
