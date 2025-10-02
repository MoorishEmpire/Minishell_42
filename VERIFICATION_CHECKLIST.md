# Minishell Integration Verification Checklist

## Pre-Compilation Checks

### File Structure
- [ ] All execution files are in `execution/` directory
- [ ] All parsing files are in `parsing/` directory (or your structure)
- [ ] `main.c` is in project root
- [ ] `execution.h` is in project root or execution directory
- [ ] `includes.h` is in project root
- [ ] `minishell.h` is in project root

### Required Files Present
- [ ] `execution/builtins/builtins.c`
- [ ] `execution/builtins/cd.c`
- [ ] `execution/builtins/echo.c`
- [ ] `execution/builtins/env.c`
- [ ] `execution/builtins/exit.c`
- [ ] `execution/builtins/export.c`
- [ ] `execution/builtins/pwd.c`
- [ ] `execution/builtins/unset.c`
- [ ] `execution/bibex.c`
- [ ] `execution/bibex_help.c`
- [ ] `execution/bibex_pipeline.c`
- [ ] `execution/external.c`
- [ ] `execution/external_helper.c`
- [ ] `execution/external_help1.c`
- [ ] `execution/heredoc.c`
- [ ] `execution/heredoc_help.c`
- [ ] `execution/signals.c`
- [ ] `execution/signals_help.c`
- [ ] `execution/help.c`
- [ ] `execution/useless_help.c`
- [ ] `execution/utils.c`
- [ ] `execution/cleanup.c`

## Compilation Checks

### Makefile Updated
- [ ] Added `execution/cleanup.c` to sources
- [ ] Added `execution/utils.c` to sources
- [ ] Included `-lreadline` flag
- [ ] Included libft library
- [ ] All execution files in compilation list
- [ ] All parsing files in compilation list

### Compilation Success
- [ ] `make` completes without errors
- [ ] No "undefined reference" errors
- [ ] No "conflicting types" warnings
- [ ] Executable `minishell` created

## Runtime Checks - Basic Commands

### Simple Commands
```bash
Test: echo hello
Expected: hello
Result: [ ]

Test: pwd
Expected: /current/directory
Result: [ ]

Test: ls
Expected: file listing
Result: [ ]

Test: ls -la
Expected: detailed listing
Result: [ ]
```

### Environment
```bash
Test: env
Expected: environment variables listed
Result: [ ]

Test: echo $HOME
Expected: your home path
Result: [ ]

Test: echo $USER
Expected: your username
Result: [ ]

Test: echo $?
Expected: 0 (or last exit status)
Result: [ ]
```

## Builtin Commands

### cd
```bash
Test: pwd
      cd /tmp
      pwd
Expected: /tmp
Result: [ ]

Test: cd ~
      pwd
Expected: home directory
Result: [ ]

Test: cd -
Expected: previous directory + prints path
Result: [ ]

Test: cd nonexistent
Expected: error message
Result: [ ]
```

### export
```bash
Test: export TEST=hello
      echo $TEST
Expected: hello
Result: [ ]

Test: export TEST=world
      echo $TEST
Expected: world
Result: [ ]

Test: export
Expected: list of exported variables
Result: [ ]

Test: export 123=invalid
Expected: error message
Result: [ ]
```

### unset
```bash
Test: export TEST=hello
      unset TEST
      echo $TEST
Expected: (empty)
Result: [ ]

Test: unset PATH
      echo $PATH
Expected: (empty)
Result: [ ]
```

### exit
```bash
Test: exit
Expected: shell exits with status 0
Result: [ ]

Test: exit 42
      echo $?  (in new shell after restart)
Expected: exits with status 42
Result: [ ]

Test: exit hello
Expected: error message + exits with 255
Result: [ ]

Test: exit 1 2
Expected: error "too many arguments" + doesn't exit
Result: [ ]
```

## Redirection Tests

### Input Redirection
```bash
Setup: echo "test content" > test.txt

Test: cat < test.txt
Expected: test content
Result: [ ]

Test: wc -l < test.txt
Expected: 1
Result: [ ]

Test: cat < nonexistent
Expected: error message
Result: [ ]
```

### Output Redirection
```bash
Test: echo hello > out.txt
      cat out.txt
Expected: hello
Result: [ ]

Test: ls -la > files.txt
      cat files.txt
Expected: file listing
Result: [ ]

Test: echo test > /invalid/path/file
Expected: error message
Result: [ ]
```

### Append Redirection
```bash
Test: echo line1 > append.txt
      echo line2 >> append.txt
      cat append.txt
Expected: line1
          line2
Result: [ ]
```

### Multiple Redirections
```bash
Setup: echo "input" > in.txt

Test: cat < in.txt > out.txt
      cat out.txt
Expected: input
Result: [ ]

Test: < in.txt cat > out.txt
      cat out.txt
Expected: input
Result: [ ]
```

## Heredoc Tests

### Basic Heredoc
```bash
Test: cat << EOF
      line1
      line2
      EOF
Expected: line1
          line2
Result: [ ]
```

### Heredoc with Variables
```bash
Test: export VAR=hello
      cat << EOF
      $VAR world
      EOF
Expected: hello world
Result: [ ]
```

### Quoted Heredoc (no expansion)
```bash
Test: cat << 'EOF'
      $HOME
      EOF
Expected: $HOME
Result: [ ]
```

### Heredoc with Command
```bash
Test: cat << EOF | grep line2
      line1
      line2
      line3
      EOF
Expected: line2
Result: [ ]
```

## Pipe Tests

### Simple Pipe
```bash
Test: echo hello | cat
Expected: hello
Result: [ ]

Test: ls | grep minishell
Expected: minishell
Result: [ ]

Test: echo "line1\nline2\nline3" | wc -l
Expected: 3
Result: [ ]
```

### Multiple Pipes
```bash
Test: ls | grep .c | wc -l
Expected: number of .c files
Result: [ ]

Test: cat /etc/passwd | grep root | cut -d: -f1
Expected: root
Result: [ ]
```

### Pipe with Redirections
```bash
Test: cat < input.txt | grep test > output.txt
      cat output.txt
Expected: lines containing 'test'
Result: [ ]

Test: echo test | cat > out.txt
      cat out.txt
Expected: test
Result: [ ]
```

## Quote Tests

### Single Quotes
```bash
Test: echo 'hello world'
Expected: hello world
Result: [ ]

Test: echo '$HOME'
Expected: $HOME
Result: [ ]

Test: echo 'test "nested" quotes'
Expected: test "nested" quotes
Result: [ ]
```

### Double Quotes
```bash
Test: echo "hello world"
Expected: hello world
Result: [ ]

Test: echo "$HOME"
Expected: /your/home/path
Result: [ ]

Test: echo "test 'nested' quotes"
Expected: test 'nested' quotes
Result: [ ]
```

### Mixed Quotes
```bash
Test: echo "hello"' world'
Expected: hello world
Result: [ ]

Test: echo 'single'"double"'single'
Expected: singledoublesingle
Result: [ ]
```

### Unclosed Quotes
```bash
Test: echo "hello
      world"
Expected: hello
          world
Result: [ ]

Test: echo 'hello
      world'
Expected: hello
          world
Result: [ ]
```

## Wildcard Tests

### Basic Wildcard
```bash
Setup: touch file1.txt file2.txt test.c

Test: ls *.txt
Expected: file1.txt file2.txt
Result: [ ]

Test: ls *.c
Expected: test.c
Result: [ ]

Test: ls *.xyz
Expected: *.xyz (literal if no match)
Result: [ ]
```

## Signal Tests

### Ctrl+C (SIGINT)
```bash
Test: (press Ctrl+C at prompt)
Expected: ^C
          new prompt
Result: [ ]

Test: cat (then Ctrl+C)
Expected: ^C
          new prompt
Result: [ ]

Test: sleep 10 (then Ctrl+C)
Expected: ^C
          new prompt
Result: [ ]
```

### Ctrl+D (EOF)
```bash
Test: (press Ctrl+D at prompt)
Expected: exit
          shell exits
Result: [ ]
```

### Ctrl+\ (SIGQUIT)
```bash
Test: (press Ctrl+\ at prompt)
Expected: ignored, nothing happens
Result: [ ]

Test: cat (then Ctrl+\)
Expected: ^\Quit: 3
          new prompt
Result: [ ]
```

## Error Handling Tests

### Command Not Found
```bash
Test: nonexistentcommand
Expected: minishell: nonexistentcommand: command not found
          exit status: 127
Result: [ ]
```

### Permission Denied
```bash
Setup: touch noperm.sh
       chmod 000 noperm.sh

Test: ./noperm.sh
Expected: minishell: ./noperm.sh: Permission denied
          exit status: 126
Result: [ ]
```

### File Not Found
```bash
Test: cat nonexistent.txt
Expected: cat: nonexistent.txt: No such file or directory
Result: [ ]

Test: cat < nonexistent.txt
Expected: minishell: nonexistent.txt: No such file or directory
Result: [ ]
```

## Memory Tests

### Valgrind Check
```bash
Test: valgrind --leak-check=full ./minishell
      (run a few commands, then exit)
Expected: "All heap blocks were freed"
          "no leaks are possible"
Result: [ ]
```

### Long Running Test
```bash
Test: (run 100+ commands in a loop)
Expected: no memory growth
          consistent behavior
Result: [ ]
```

## Edge Cases

### Empty Input
```bash
Test: (press enter with no input)
Expected: new prompt
Result: [ ]

Test: "    " (spaces only)
Expected: new prompt
Result: [ ]
```

### Special Characters
```bash
Test: echo ';'
Expected: ;
Result: [ ]

Test: echo '&&'
Expected: &&
Result: [ ]

Test: echo '||'
Expected: ||
Result: [ ]
```

### Long Commands
```bash
Test: echo (very long string, 1000+ chars)
Expected: string echoed correctly
Result: [ ]
```

### Many Pipes
```bash
Test: cat file | cat | cat | cat | cat
Expected: file contents
Result: [ ]
```

## Integration Tests

### Complex Command 1
```bash
Test: export NAME=test
      echo $NAME | cat > file.txt
      cat < file.txt
Expected: test
Result: [ ]
```

### Complex Command 2
```bash
Test: cat << EOF | grep line | wc -l > count.txt
      line 1
      line 2
      other
      line 3
      EOF
      cat count.txt
Expected: 3
Result: [ ]
```

### Complex Command 3
```bash
Test: ls -la | grep minishell > ls_out.txt
      wc -l < ls_out.txt
Expected: 1 (or number of matches)
Result: [ ]
```

## Final Checklist

### Functionality
- [ ] All builtins work correctly
- [ ] External commands execute
- [ ] Redirections work (>, <, >>)
- [ ] Heredocs work (<<)
- [ ] Pipes work (|)
- [ ] Environment variables expand
- [ ] Quotes handled correctly
- [ ] Wildcards expand correctly

### Error Handling
- [ ] Command not found errors
- [ ] File not found errors
- [ ] Permission errors
- [ ] Syntax errors
- [ ] Memory allocation failures handled

### Signals
- [ ] Ctrl+C works in interactive mode
- [ ] Ctrl+C works during command execution
- [ ] Ctrl+D exits shell
- [ ] Ctrl+\ handled correctly

### Memory
- [ ] No memory leaks (valgrind clean)
- [ ] All allocated memory freed
- [ ] No double frees
- [ ] No use after free

### Exit Status
- [ ] $? reflects last command status
- [ ] 0 for success
- [ ] 127 for command not found
- [ ] 126 for permission denied
- [ ] 130 for Ctrl+C

## Score

Total Tests: _____
Passed: _____
Failed: _____
Success Rate: _____%

## Notes

Add any issues or observations here:

_______________________________________________________________________________

_______________________________________________________________________________

_______________________________________________________________________________

