
.PHONY: all clean fclean re

NAME = philo_three

CC = gcc

CFLAGS = -Wall -Werror -Wextra

HEADER_DIR = srcs

SRC_DIR = ./srcs

SRC = ft_string.c	life_utils.c	life.c	processing.c	philo_three.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC))

OBJS = $(SRCS:%.c=%.o)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(HEADER_DIR) -lpthread

%.o : %.c
		$(CC) $(CFLAGS) -o $@ -c $^ -I $(HEADER_DIR)

all : $(NAME)

clean :
		rm -f $(OBJS)

fclean : clean
		rm -f $(NAME)

re : fclean all