# Minishell Makefile
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./utils/libft
LIBS = -lreadline -L./utils/libft -lft

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Source files - Builtins
BUILTIN_SRCS = execution/builtins/builtins.c \
               execution/builtins/cd.c \
               execution/builtins/echo.c \
               execution/builtins/env.c \
               execution/builtins/exit.c \
               execution/builtins/export.c \
               execution/builtins/pwd.c \
               execution/builtins/unset.c

# Source files - Execution
EXEC_SRCS = execution/bibex.c \
            execution/bibex_help.c \
            execution/bibex_pipeline.c \
            execution/external.c \
            execution/external_helper.c \
            execution/external_help1.c \
            execution/heredoc.c \
            execution/heredoc_help.c \
            execution/signals.c \
            execution/signals_help.c \
            execution/help.c \
            execution/useless_help.c \
            execution/utils.c \
            execution/cleanup.c

# Source files - Parsing (adjust paths to match your structure)
PARSE_SRCS = parsing/parsing.c \
             parsing/token.c \
             parsing/tokenizer.c \
             parsing/stripper.c \
             var_expansion/arg_replace.c \
             var_expansion/variables.c \
             var_expansion/misc.c \
             var_expansion/norm.c \
             var_expansion/support.c \
             wild_card/file_read.c \
             error_checks/pipe_err_check.c \
             error_checks/redirection_er.c \
             error_checks/side_functions.c \
             utils/helper_funcs/helps.c

# Main source
MAIN_SRC = main.c

# All sources
SRCS = $(MAIN_SRC) $(BUILTIN_SRCS) $(EXEC_SRCS) $(PARSE_SRCS)

# Object files
OBJS = $(SRCS:.c=.o)

# Libft
LIBFT_DIR = utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(GREEN)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(RESET)"

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

# Testing targets
test: $(NAME)
	@echo "$(GREEN)Running basic tests...$(RESET)"
	@./$(NAME)

# Debugging with valgrind
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

.PHONY: all clean fclean re test valgrind
