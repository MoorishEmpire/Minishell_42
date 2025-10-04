# Minishell - Integrated Parsing & Execution System

A bash-like shell implementation with full parsing and execution capabilities.

## 🎯 Project Status

✅ **INTEGRATION COMPLETE**

- ✓ Parsing system integrated
- ✓ Execution system adapted to new `t_cmd` structure
- ✓ All builtins implemented
- ✓ Pipeline execution working
- ✓ Redirection handling updated
- ✓ Signal management implemented
- ✓ Memory cleanup functions added

## 📚 Documentation Index

Start here for quick onboarding:

### 🚀 Getting Started
1. **[QUICK_START.md](QUICK_START.md)** - Start here! Quick setup and compilation guide
2. **[CHANGES_SUMMARY.txt](CHANGES_SUMMARY.txt)** - Overview of all changes made

### 🏗️ Architecture
3. **[INTEGRATION_GUIDE.md](INTEGRATION_GUIDE.md)** - Detailed system architecture and data flow
4. **[ARCHITECTURE_DIAGRAM.txt](ARCHITECTURE_DIAGRAM.txt)** - Visual diagrams of system components
5. **[MIGRATION_SUMMARY.md](MIGRATION_SUMMARY.md)** - Explanation of struct changes

### ✅ Testing
6. **[VERIFICATION_CHECKLIST.md](VERIFICATION_CHECKLIST.md)** - Comprehensive testing checklist

### 📖 Reference
7. **[execution.h](execution.h)** - All execution function prototypes
8. **[includes.h](includes.h)** - All parsing function prototypes
9. **[Makefile_example](Makefile_example)** - Compilation template

## 🏃 Quick Start

```bash
# 1. Update your Makefile (or use the example)
cp Makefile_example Makefile

# 2. Compile
make

# 3. Run
./minishell

# 4. Test
minishell > echo hello world
minishell > ls -la | grep txt
minishell > cat << EOF
> test heredoc
> EOF
```

## 📁 Project Structure

```
minishell/
├── main.c                      # Integrated main function
├── minishell.h                 # Master header
├── execution.h                 # Execution system header
├── includes.h                  # Parsing system header
│
├── execution/                  # Execution system
│   ├── builtins/              # Built-in commands
│   │   ├── builtins.c         # Builtin dispatcher
│   │   ├── cd.c               # Change directory
│   │   ├── echo.c             # Echo command
│   │   ├── env.c              # Environment management
│   │   ├── exit.c             # Exit shell
│   │   ├── export.c           # Export variables
│   │   ├── pwd.c              # Print working directory
│   │   └── unset.c            # Unset variables
│   │
│   ├── bibex.c                # Pipeline execution
│   ├── bibex_help.c           # Pipeline helpers
│   ├── bibex_pipeline.c       # Pipeline management
│   ├── external.c             # External command execution
│   ├── external_helper.c      # Command path resolution
│   ├── external_help1.c       # Error handling
│   ├── heredoc.c              # Heredoc processing
│   ├── heredoc_help.c         # Heredoc helpers
│   ├── signals.c              # Signal handling
│   ├── signals_help.c         # Signal utilities
│   ├── help.c                 # Pipeline utilities
│   ├── useless_help.c         # General utilities
│   ├── utils.c                # Helper functions
│   └── cleanup.c              # Memory management
│
└── parsing/                    # Your parsing system
    ├── tokenizer.c            # Token creation
    ├── expander.c             # Variable expansion
    ├── wildcard.c             # Wildcard expansion
    ├── parser.c               # Command structure builder
    └── ...
```

## 🔄 Data Flow

```
User Input
    ↓
Tokenizer → Raw tokens
    ↓
Expander → Variables expanded
    ↓
Wildcard → Wildcards expanded
    ↓
Stripper → Quotes removed
    ↓
Parser → t_cmd structures
    ↓
Executor → Command execution
```

## 🔑 Key Features

### Implemented Features
- ✅ **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- ✅ **Redirections**: `<`, `>`, `>>`
- ✅ **Heredoc**: `<<` with variable expansion
- ✅ **Pipes**: Multiple command pipelines
- ✅ **Variables**: Environment variable expansion
- ✅ **Quotes**: Single and double quote handling
- ✅ **Wildcards**: `*` expansion
- ✅ **Signals**: Ctrl+C, Ctrl+D, Ctrl+\ handling
- ✅ **Exit Status**: `$?` support

### Command Examples
```bash
# Simple commands
echo hello world
ls -la
pwd

# Redirections
cat < input.txt > output.txt
echo test >> file.txt

# Heredoc
cat << EOF
content here
$HOME expansion
EOF

# Pipes
ls | grep txt | wc -l
cat file | grep pattern > result

# Variables
export NAME=value
echo $NAME
unset NAME

# Quotes
echo "hello $USER"
echo 'literal $HOME'

# Wildcards
ls *.c
cat test*.txt
```

## 🔧 Main Components

### Command Structure (`t_cmd`)
```c
typedef struct s_cmd
{
    char **argv;        // ["command", "arg1", "arg2", NULL]
    char **redirect;    // ["<", ">", NULL]
    char **file;        // ["in.txt", "out.txt", NULL]
    struct s_cmd *next; // Next command in pipeline
} t_cmd;
```

### Environment (`t_env`)
```c
typedef struct s_env
{
    char *var;          // Variable name
    char *value;        // Variable value
    struct s_env *next; // Next variable
} t_env;
```

### Token (`t_token`)
```c
typedef struct s_token
{
    char *value;        // Token content
    char *var_nam;      // Variable name (if applicable)
    NodeType type;      // Token type
    int quote_flag;     // Quote status
    struct s_token *next;
} t_token;
```

## 🎯 Execution Flow

### Single Command
```
Input: "ls -la > output.txt"
    ↓
Parse to t_cmd
    ↓
Check if builtin → No
    ↓
Fork process
    ↓
Apply redirections
    ↓
Execute with execve()
    ↓
Wait and get exit status
```

### Pipeline
```
Input: "cat file | grep test | wc -l"
    ↓
Parse to 3 t_cmd nodes
    ↓
Create 2 pipes
    ↓
Fork 3 children
    ↓
Connect pipes: cmd1→pipe1→cmd2→pipe2→cmd3
    ↓
Execute all commands
    ↓
Wait for all, get last exit status
```

## 🧪 Testing

### Quick Test
```bash
# Run a few commands
./minishell
minishell > echo test
minishell > ls | grep minishell
minishell > exit
```

### Comprehensive Test
```bash
# Use the verification checklist
# See VERIFICATION_CHECKLIST.md
```

### Memory Test
```bash
# Check for leaks
valgrind --leak-check=full ./minishell
# Run some commands, then exit
minishell > echo test
minishell > ls
minishell > exit
```

## 📋 Function Reference

### Main Entry Points
- `main()` - Main loop and initialization
- `process_line()` - Process single input line
- `execute_command()` - Execute command dispatcher

### Parsing (Your Code)
- `tokenizer()` - Create tokens from input
- `expanding_it()` - Expand variables
- `handel_wild_card()` - Expand wildcards
- `stripper()` - Remove quotes
- `build_cmd_list()` - Build command structure

### Execution
- `execute_pipeline()` - Handle piped commands
- `execute_builtin()` - Run builtin commands
- `exec_externals()` - Run external commands
- `collecting_heredoc()` - Process heredocs
- `apply_redirection()` - Handle file redirections

### Builtins
- `ft_echo()` - Echo command
- `ft_cd()` - Change directory
- `ft_pwd()` - Print working directory
- `ft_env()` - Print environment
- `ft_export()` - Export variables
- `ft_unset()` - Unset variables
- `ft_exit()` - Exit shell

### Utilities
- `is_builtin()` - Check if command is builtin
- `get_cmd_path()` - Resolve command path
- `get_env_value()` - Get environment variable
- `update_env_var()` - Update environment variable

### Cleanup
- `clear_tokens()` - Free token list
- `clear_cmd()` - Free command list
- `free_env()` - Free environment list
- `free_2d_arr()` - Free string arrays

## 🐛 Debugging

### Enable Debug Output
In `main.c`, uncomment:
```c
if (cmd)
    print_parse(cmd);  // Shows parsed structure
```

### Check Specific Component
```c
print_tokens(output);   // After tokenizer
print_tokens(expand);   // After expander
print_tokens(wild);     // After wildcard
print_tokens(strip);    // After stripper
print_parse(cmd);       // After parser
```

### Common Issues

**Command not found**
- Check `$PATH` environment variable
- Verify command exists and is executable

**Redirection fails**
- Check file permissions
- Verify directory exists

**Memory leaks**
- Run with valgrind
- Check all malloc/free pairs
- Verify cleanup functions are called

**Segmentation fault**
- Check NULL pointer dereferences
- Verify array bounds
- Check malloc failures

## 📝 Exit Codes

- `0` - Success
- `1` - General error
- `126` - Command found but not executable
- `127` - Command not found
- `130` - Terminated by Ctrl+C
- `255` - Exit status out of range

## 🔒 Signal Handling

### Interactive Mode
- **Ctrl+C (SIGINT)**: Display new prompt
- **Ctrl+\ (SIGQUIT)**: Ignored
- **Ctrl+D (EOF)**: Exit shell

### Command Execution
- **Ctrl+C**: Terminate command, show new prompt
- **Ctrl+\**: Terminate command with core dump message

### Heredoc
- **Ctrl+C**: Cancel heredoc input
- **Ctrl+D**: End heredoc input

## 📊 Compilation

### Requirements
- GCC or Clang
- readline library (`-lreadline`)
- libft (your implementation)

### Flags
```makefile
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline
```

### Compile
```bash
make          # Compile project
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Recompile everything
```

## 🤝 Integration Points

Your parsing code must provide:
1. `build_cmd_list(t_token *)` - Returns `t_cmd *`
2. `t_cmd` with properly filled:
   - `argv` array
   - `redirect` array (parallel to file)
   - `file` array (parallel to redirect)
   - `next` pointer (for pipes)

The execution system will handle the rest.

## 📚 Additional Resources

- **Bash Manual**: For reference behavior
- **POSIX Specification**: For standard compliance
- **42 Minishell Subject**: For project requirements

## ✨ Credits

Execution system adapted for new `t_cmd` structure.
Integration completed with full parsing pipeline support.

---

**Need Help?** Check the documentation files listed above, especially:
1. `QUICK_START.md` for immediate setup
2. `INTEGRATION_GUIDE.md` for understanding the system
3. `VERIFICATION_CHECKLIST.md` for testing

**Ready to start?** Run: `make && ./minishell`
