
# COLORS #

# This is a minimal set of ANSI color codes
_END		=	\033[0;0m
_BOLD		=	\033[0;1m
_UNDER		=	\033[0;4m
_REV		=	\033[0;7m

# Colors
_GREY		=	\033[0;30m
_RED		=	\033[0;31m
_GREEN		=	\033[0;32m
_YELLOW		=	\033[0;33m
_BLUE		=	\033[0;34m
_PURPLE		=	\033[0;35m
_CYAN		=	\033[0;36m
_WHITE		=	\033[0;37m

# Inverted, i.e. colored backgrounds
_IGREY		=	\033[0;40m
_IRED		=	\033[0;41m
_IGREEN		=	\033[0;42m
_IYELLOW	=	\033[0;43m
_IBLUE		=	\033[0;44m
_IPURPLE	=	\033[0;45m
_ICYAN		=	\033[0;46m
_IWHITE		=	\033[0;47m

.PHONY: all clean fclean re

NAME = philo_one

CC = gcc

CFLAGS = -Wall -Werror -Wextra

HEADER_DIR = srcs

SRC = ft_string.c	ft_strjoin.c	philosopher.c

SRCS = $(addprefix ./srcs/, $(SRC))

OBJS = $(SRCS:%.c=%.o)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(HEADER_DIR)
	@echo "$(_GREEN)Program '$(NAME)' compiled.$(_END)"

%.o : %.c
		@$(CC) $(CFLAGS) -o $@ -c $^ -I $(HEADER_DIR)

all : $(NAME)

clean :
		rm -f $(OBJS)

fclean : clean
		rm -f $(NAME)

re : fclean all