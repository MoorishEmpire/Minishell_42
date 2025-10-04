# Final Integration Guide

## ✅ What's Been Done

1. **Unified Header**: Created `/workspace/includes/minishell.h`
   - Contains ALL structures and function prototypes
   - Both parsing and execution use this single header

2. **Updated All Files**:
   - All execution files: `#include "../includes/minishell.h"`
   - Main.c: `#include "includes/minishell.h"`
   - Created Makefile with all source files

3. **Created Stripper**: `/workspace/parsing/stripper.c`

## 📋 Files You Need to Add Headers To

Add `#include "../includes/minishell.h"` to the top of these files:

### Tokenization Files
```bash
parsing/token.c
parsing/tokenizer.c
```

### Variable Expansion Files
```bash
var_expansion/arg_replace.c
var_expansion/variables.c
var_expansion/misc.c
var_expansion/norm.c
var_expansion/support.c
```

### Wildcard Files
```bash
wild_card/file_read.c
```

### Error Checking Files
```bash
error_checks/pipe_err_check.c
error_checks/redirection_er.c
error_checks/side_functions.c
```

### Helper Files
```bash
utils/helper_funcs/helps.c
```

## 🚀 Quick Setup Script

Run this command to add headers to all your parsing files:

```bash
# Add header to all parsing-related files
for file in parsing/*.c var_expansion/*.c wild_card/*.c error_checks/*.c utils/helper_funcs/*.c; do
    if [ -f "$file" ]; then
        # Check if header not already there
        if ! grep -q '#include.*minishell.h' "$file"; then
            # Create temp file with header + original content
            echo '#include "../includes/minishell.h"' > temp_file
            echo '' >> temp_file
            cat "$file" >> temp_file
            mv temp_file "$file"
            echo "Added header to $file"
        fi
    fi
done
```

## 📁 Directory Structure Expected

```
/workspace/
├── includes/
│   └── minishell.h          ← Unified header
├── main.c
├── Makefile
├── execution/
│   ├── builtins/
│   │   └── *.c
│   └── *.c
├── parsing/
│   ├── parsing.c
│   ├── token.c
│   ├── tokenizer.c
│   └── stripper.c
├── var_expansion/
│   ├── arg_replace.c
│   ├── variables.c
│   ├── misc.c
│   ├── norm.c
│   └── support.c
├── wild_card/
│   └── file_read.c
├── error_checks/
│   ├── pipe_err_check.c
│   ├── redirection_er.c
│   └── side_functions.c
└── utils/
    ├── helper_funcs/
    │   └── helps.c
    └── libft/
        └── (your libft files)
```

## 🔧 Compilation Steps

1. **Update your parsing files with the header**:
   ```bash
   # Run the script above
   ```

2. **Compile libft**:
   ```bash
   cd utils/libft
   make
   cd ../..
   ```

3. **Compile minishell**:
   ```bash
   make
   ```

4. **Run**:
   ```bash
   ./minishell
   ```

## 🐛 Common Issues & Fixes

### Issue: "No such file or directory" for minishell.h
**Fix**: Ensure the file is at `/workspace/includes/minishell.h`

### Issue: Duplicate definitions
**Fix**: Remove old `includes.h` or `execution.h` includes from your files

### Issue: get_env_value conflicts
Your parsing code has `char *get_env_value(char *name, char **env)`  
Execution code has `char *get_env_value(t_env *env, const char *key)`

These are different functions! Rename one:
```c
// In your var_expansion code:
char *get_env_value_from_array(char *name, char **env)
```

Then update calls in:
- var_expansion/arg_replace.c
- var_expansion/misc.c

### Issue: Compilation errors about missing functions
Check that the function is declared in `minishell.h`

## 🎯 Testing After Compilation

```bash
# Basic test
./minishell
minishell > echo hello
minishell > pwd
minishell > ls | grep minishell
minishell > exit

# If it works, you're done!
```

## 📝 Final Checklist

- [ ] All parsing files have `#include "../includes/minishell.h"`
- [ ] All execution files have correct include path
- [ ] Renamed conflicting `get_env_value` function
- [ ] libft compiled
- [ ] Makefile has all source files listed
- [ ] Project compiles without errors
- [ ] Basic commands work

## 🎉 You're Ready!

Once all files have the correct header and compile successfully, your minishell is fully integrated and ready to use!

The key changes from your original code:
1. Single unified header (`minishell.h`)
2. All `t_cmd` structures use `argv`, `redirect[]`, `file[]`
3. Both parsing and execution share the same structures
4. Main.c orchestrates the full pipeline

Good luck! 🚀
