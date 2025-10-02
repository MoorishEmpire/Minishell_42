NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAGS = -lreadline

# Source files
SRCS = main.c \
       utils/libft.c \
       parsing/parsing.c \
       parsing/tokenizer.c \
       parsing/token.c \
       parsing/helper_funcs.c \
       parsing/var_expansion.c \
       parsing/variables.c \
       parsing/misc.c \
       parsing/support.c \
       parsing/wildcard.c \
       parsing/pipe_err_check.c \
       parsing/redirection_err.c \
       parsing/side_functions.c \
       parsing/stripper.c \
       builtins/builtin.c \
       builtins/cd.c \
       builtins/echo.c \
       builtins/env.c \
       builtins/exit.c \
       builtins/export.c \
       builtins/pwd.c \
       builtins/unset.c \
       execution/Bibex.c \
       execution/bibex_help.c \
       execution/bibex_pipeline.c \
       execution/builtin_utils.c \
       execution/external.c \
       execution/external_helper.c \
       execution/external_help1.c \
       execution/ft_wildcard.c \
       execution/help.c \
       execution/Heredoc.c \
       execution/heredoc_help.c \
       execution/redirection_converter.c \
       execution/Signals.c \
       execution/signals_help.c \
       execution/useless_help.c

# Object files
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(READLINE_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
# If using libft:
# 	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
# If using libft:
# 	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re