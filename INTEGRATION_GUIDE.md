# Minishell Integration Guide

## Overview
This guide explains how your parsing and execution systems are integrated.

## File Structure

```
workspace/
├── main.c                          # Main entry point
├── minishell.h                     # Master header
├── includes.h (parsing.h)          # Parsing system header
├── execution.h                     # Execution system header
│
├── execution/                      # Execution system
│   ├── builtins/
│   │   ├── builtins.c
│   │   ├── cd.c
│   │   ├── echo.c
│   │   ├── env.c
│   │   ├── exit.c
│   │   ├── export.c
│   │   ├── pwd.c
│   │   └── unset.c
│   │
│   ├── bibex.c                     # Pipeline execution
│   ├── bibex_help.c
│   ├── bibex_pipeline.c
│   ├── external.c                  # External command execution
│   ├── external_helper.c
│   ├── external_help1.c
│   ├── heredoc.c                   # Heredoc handling
│   ├── heredoc_help.c
│   ├── signals.c                   # Signal management
│   ├── signals_help.c
│   ├── help.c                      # Pipeline helpers
│   ├── useless_help.c              # Utility functions
│   ├── utils.c                     # Redirection type helpers
│   └── cleanup.c                   # Memory cleanup functions
│
└── parsing/                        # Your parsing system
    ├── tokenizer.c
    ├── expander.c
    ├── wildcard.c
    ├── parser.c
    └── ...
```

## Data Flow

### 1. Main Loop (main.c)
```
User Input
    ↓
read_complete_line()  ← handles unclosed quotes
    ↓
process_line()
```

### 2. Process Line Flow
```
Input String
    ↓
tokenizer()           → t_token list (raw tokens)
    ↓
expanding_it()        → t_token list (variables expanded)
    ↓
handel_wild_card()    → t_token list (wildcards expanded)
    ↓
stripper()            → t_token list (quotes stripped)
    ↓
build_cmd_list()      → t_cmd list (commands structured)
    ↓
execute_command()     → execution
```

### 3. Execution Flow
```
t_cmd list
    ↓
collecting_heredoc()  ← process all heredocs first
    ↓
if (multiple commands)
    execute_pipeline()
else if (builtin)
    execute_builtin()
else
    exec_externals()
```

## Key Structures

### t_cmd (Shared)
```c
typedef struct s_cmd
{
    char **argv;        // Command and arguments: ["ls", "-la", NULL]
    char **redirect;    // Redirection operators: ["<", ">", NULL]
    char **file;        // Corresponding files: ["in.txt", "out.txt", NULL]
    struct s_cmd *next; // Next command in pipeline
} t_cmd;
```

### t_env (Execution)
```c
typedef struct s_env
{
    char *var;          // Variable name: "PATH"
    char *value;        // Variable value: "/usr/bin:/bin"
    struct s_env *next; // Next variable
} t_env;
```

### t_token (Parsing)
```c
typedef struct s_token
{
    char *value;        // Token value
    char *var_nam;      // Variable name (if applicable)
    NodeType type;      // Token type
    int quote_flag;     // Quote status
    struct s_token *next;
} t_token;
```

## Main Function Breakdown

```c
int main(int ac, char **av, char **env)
{
    // 1. Initialize
    g_exit_status = 0;
    env_list = env_to_list(env);        // Convert env to linked list
    set_signals_interactive();          // Setup signal handlers
    
    // 2. Main loop
    while (1)
    {
        input = read_complete_line();   // Read with quote handling
        if (!input)
            exit_fun(env_list);         // EOF (Ctrl+D)
        
        if (*input)
            process_line(input, env, &env_list);
        
        free(input);
    }
    
    // 3. Cleanup
    clear_history();
    free_env(env_list);
    return (g_exit_status);
}
```

## Execution Decision Tree

```
execute_command(t_cmd *cmd)
    │
    ├─→ collecting_heredoc()
    │   ├─→ if error: return
    │   └─→ continue
    │
    ├─→ if (cmd->next != NULL)
    │   └─→ execute_pipeline()      // Multiple commands
    │       ├─→ fork each command
    │       ├─→ setup pipes
    │       ├─→ apply redirections
    │       └─→ wait for all
    │
    └─→ else (single command)
        ├─→ if (is_builtin(cmd->argv[0]))
        │   └─→ execute_builtin()   // Run in parent process
        │
        └─→ else
            └─→ exec_externals()    // Fork and exec
```

## Important Functions

### Parsing → Execution Interface
- `build_cmd_list(t_token *strip)` → Creates `t_cmd` list from tokens
- Returns: `t_cmd *` ready for execution

### Execution Entry Points
- `execute_command(t_cmd *cmd, t_env **env_list)` - Main dispatcher
- `execute_pipeline(t_cmd *cmds, t_env *env)` - Handle pipelines
- `execute_builtin(t_cmd *cmd, t_env **env)` - Run builtins
- `exec_externals(t_cmd *cmd, t_env *env)` - Run external commands

### Memory Management
- `clear_tokens(t_token **head)` - Free token list
- `clear_cmd(t_cmd **head)` - Free command list
- `free_env(t_env *env)` - Free environment list
- `free_2d_arr(char **arr)` - Free string arrays

## Signal Handling

### Three Signal Contexts
1. **Interactive** (waiting for input)
   - `set_signals_interactive()`
   - SIGINT (Ctrl+C): New prompt
   - SIGQUIT (Ctrl+\): Ignored

2. **Child Execution** (parent waiting)
   - `set_signals_for_child_execution()`
   - Both signals ignored in parent

3. **Child Process** (actual command)
   - `set_signals_in_child()`
   - Default signal behavior

4. **Heredoc** (reading heredoc input)
   - `set_signals_heredoc()`
   - SIGINT: Close and exit

## Redirection Handling

### How it Works
```c
// Command: cat < in.txt > out.txt
// Results in:
cmd->redirect = ["<", ">", NULL]
cmd->file     = ["in.txt", "out.txt", NULL]

// In apply_redirection():
int i = 0;
while (cmd->redirect[i] && cmd->file[i])
{
    if (redirect[i] == "<")
        fd = open(file[i], O_RDONLY);
        dup2(fd, STDIN_FILENO);
    else if (redirect[i] == ">")
        fd = open(file[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
    // ... etc
    i++;
}
```

## Heredoc Handling

Heredocs are special:
1. **Collected BEFORE execution** in `collecting_heredoc()`
2. **Forked process** reads input line by line
3. **Written to pipe**, fd stored for later
4. **During execution**, pipe fd is dup2'd to STDIN

## Pipeline Execution

```c
// Command: ls | grep txt | wc -l
// Results in 3 t_cmd nodes linked by ->next

execute_pipeline():
    1. Create pipes: pipe[0], pipe[1]
    2. Fork 3 children
    3. Each child:
       - Redirects: prev_pipe[0] → STDIN
       - Redirects: next_pipe[1] → STDOUT
       - Closes unused pipes
       - Applies file redirections
       - Executes command
    4. Parent closes all pipes
    5. Parent waits for all children
```

## Environment Variables

Two representations:
1. **Array** (`char **env`) - System format
2. **List** (`t_env *`) - Minishell format

Conversion functions:
- `env_to_list(char **env)` → `t_env *`
- `list_to_env(t_env *list)` → `char **`

## Builtins vs External

**Builtins** (executed in parent):
- `cd`, `export`, `unset`, `exit`
- Must run in parent to affect shell state

**Can fork** (no state change):
- `echo`, `pwd`, `env`

**External commands**:
- Everything else
- Always forked

## Error Handling

Exit codes follow bash conventions:
- `0` - Success
- `1` - General errors
- `2` - Misuse of shell builtins
- `126` - Command found but not executable
- `127` - Command not found
- `128 + N` - Command terminated by signal N
- `130` - Ctrl+C (128 + SIGINT)
- `255` - Exit status out of range

## Debugging Tips

### Enable Debug Output
Uncomment in main.c:
```c
if (cmd)
    print_parse(cmd);  // Show parsed structure
```

### Check Memory Leaks
```bash
valgrind --leak-check=full --show-leak-kinds=all ./minishell
```

### Test Individual Components
1. Tokenizer: Uncomment `print_tokens(output)`
2. Expander: Uncomment `print_tokens(expand)`
3. Wildcard: Uncomment `print_tokens(wild)`
4. Parser: Use `print_parse(cmd)`

## Common Issues

### Issue: Command not found
- Check `$PATH` environment variable
- Verify command is executable
- Check file permissions

### Issue: Redirection fails
- Verify file permissions
- Check if file/directory exists
- Ensure correct operator used

### Issue: Heredoc doesn't work
- Check delimiter matching
- Verify signal handling
- Ensure pipe creation succeeds

### Issue: Pipe doesn't work
- Check all commands in pipeline
- Verify pipe creation
- Ensure proper fd management

## Testing Checklist

```bash
# Basic commands
echo hello
pwd
env

# Redirections
echo test > file
cat < file
echo append >> file
cat << EOF

# Pipes
ls | grep txt
cat file | grep x | wc -l

# Builtins
cd /tmp
export VAR=value
unset VAR
exit 42

# Complex
cat < in | grep x > out
ls | grep txt >> results 2>&1
```

## Next Steps

1. Add error handling for edge cases
2. Implement proper signal handling for pipelines
3. Add support for logical operators (&&, ||)
4. Improve heredoc with proper fd management
5. Add comprehensive test suite
