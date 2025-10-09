# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-most <ael-most@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/24 15:21:00 by ael-most          #+#    #+#              #
#    Updated: 2024/11/05 23:24:05 by ael-most         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############
## VARIABLES ##
###############

NAME = libft.a
CC = cc
CCFLAGS = -Wall -Werror -Wextra
CC_COMPILER_CALL = $(CC) $(CCFLAGS) 
AR = ar
ARFLAGS = rcs
RM = rm -f
HEADERS = libft.h

###################
#### MANDATORY ####
###################

SRC = 	ft_isalpha.c ft_isdigit.c ft_isascii.c \
		ft_isprint.c ft_strlen.c ft_memset.c \
		ft_bzero.c ft_memcpy.c ft_memmove.c \
		ft_strlcpy.c ft_strlcat.c ft_toupper.c \
		ft_tolower.c ft_strchr.c ft_strrchr.c \
		ft_strncmp.c ft_memchr.c ft_memcmp.c \
		ft_isalnum.c ft_strnstr.c ft_atoi.c \
		ft_calloc.c ft_strdup.c ft_substr.c \
		ft_strjoin.c ft_strtrim.c ft_split.c \
		ft_itoa.c ft_strmapi.c ft_striteri.c \
		ft_putchar_fd.c ft_putstr_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c

OBJ = $(SRC:.c=.o)

###############
#### BONUS ####
###############

BONUS_SRC =	ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
			ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
			ft_lstclear_bonus.c ft_lstiter_bonus.c \
			ft_lstmap_bonus.c ft_lstlast_bonus.c \
			ft_lstsize_bonus.c

BONUS_OBJ = $(BONUS_SRC:.c=.o)

#############
## TARGETS ##
#############

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

bonus: $(BONUS_OBJ)
	$(AR) $(ARFLAGS) $(NAME) $(BONUS_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

################
### PATTERNS ###
################

%.o : %.c $(HEADERS)
	$(CC_COMPILER_CALL) -c $< -o $@

###############
#### PHONY ####
###############

.PHONY: clean

##################
## HELP MESSAGE ##
##################

help:
	@echo "Available targets:"
	@echo "  all     - Build the library"
	@echo "  bonus   - Build the library with bonus features"
	@echo "  clean   - Remove object files"
	@echo "  fclean  - Remove object files and the library"
	@echo "  re      - Rebuild the library"
	@echo "  help    - Show this help message"

####################
## DEFAULT TARGET ##
####################
.DEFAULT_GOAL := all
