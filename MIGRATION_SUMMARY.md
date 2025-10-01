# Migration Summary: Updated t_cmd Structure

## New Structure
```c
typedef struct s_cmd
{
    char **argv;        // Combined command and arguments
    char **redirect;    // Array of redirection operators ("<", ">", ">>", "<<")
    char **file;        // Array of corresponding filenames
    struct s_cmd *next; // Next command in pipeline
} t_cmd;
```

## Key Changes Made

### 1. Command and Arguments
**Before:**
- `cmd->cmd` - command name
- `cmd->args` - full array including command name

**After:**
- `cmd->argv[0]` - command name
- `cmd->argv` - full array including command name

### 2. Redirections
**Before:**
- Used a linked list of `t_file` structures
- Each `t_file` had: `name`, `type`, `h_fd`, `quoted`, `next`

**After:**
- `cmd->redirect[]` - array of redirection operators as strings
- `cmd->file[]` - array of corresponding filenames
- Both arrays are parallel (same index corresponds to same redirection)

### 3. Updated Files

#### Builtins (No major changes needed)
- `builtins.c` - Already uses `cmd->argv`
- `cd.c`, `echo.c`, `env.c`, `exit.c`, `export.c`, `pwd.c`, `unset.c` - All work with `argv`

#### Execution Files (Major changes)
- **bibex.c**: Changed `is_builtin(cmd->cmd)` to `is_builtin(cmd->argv[0])`
- **external.c**: 
  - Updated `apply_redirection()` to iterate through `redirect[]` and `file[]` arrays
  - Changed `cmd->cmd` to `cmd->argv[0]` in all functions
  - Changed `cmd->args` to `cmd->argv`
- **external_helper.c**: Updated all references from `cmd->cmd` to `cmd->argv[0]`
- **external_help1.c**: Updated error messages to use `cmd->argv[0]`
- **heredoc.c**: Updated to work with `redirect[]` and `file[]` arrays instead of linked list

### 4. New Helper Functions

Created `utils.c` with:
- `get_redir_type(char *redir_str)` - Converts string operators to token types
- `is_builtin(char *cmd)` - Checks if command is a builtin

### 5. Redirection Handling

**Old approach:**
```c
t_file *f = cmd->file;
while (f) {
    handle_redir(f);
    f = f->next;
}
```

**New approach:**
```c
int i = 0;
while (cmd->redirect[i] && cmd->file[i]) {
    int type = get_redir_type(cmd->redirect[i]);
    handle_redir_by_type(type, cmd->file[i]);
    i++;
}
```

### 6. Important Notes

1. **Heredoc handling**: The heredoc file descriptor (`h_fd`) storage needs to be handled differently since we no longer have the `t_file` structure. You may need to:
   - Add an array to store heredoc fds in `t_cmd`
   - Or handle heredocs differently in your implementation

2. **Token types**: The code assumes these token types exist:
   - `TOKEN_REDIR_IN` - for "<"
   - `TOKEN_REDIR_OUT` - for ">"
   - `TOKEN_APPEND` - for ">>"
   - `TOKEN_HEREDOC` - for "<<"
   - `TOKEN_HEREDOC_quoted` - for quoted heredoc delimiters

3. **Wildcard expansion**: The `ft_wildcard.c` file was included but not modified as it appears to work with tokens, not commands.

## Testing Checklist

- [ ] Simple commands: `echo hello`
- [ ] Commands with arguments: `ls -la /tmp`
- [ ] Input redirection: `cat < file.txt`
- [ ] Output redirection: `echo test > out.txt`
- [ ] Append redirection: `echo test >> out.txt`
- [ ] Heredoc: `cat << EOF`
- [ ] Multiple redirections: `cat < in.txt > out.txt`
- [ ] Pipes: `ls | grep txt`
- [ ] Multiple pipes: `cat file | grep x | wc -l`
- [ ] All builtins: `echo`, `cd`, `pwd`, `env`, `export`, `unset`, `exit`

## Compilation

Make sure to compile all new/updated files:
- All files in `execution/builtins/`
- All files in `execution/` root
- New `execution/utils.c`
