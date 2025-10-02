NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAGS = -lreadline

# Source files
SRCS = main.c \
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

# Libft
LIBFT_DIR = utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re