# Makefile
NAME    = philo
LIB     = libphilo.a

CC      = cc
CFLAGS  = -pthread -Wall -Wextra -Werror -g

SRC = 	src/init.c \
		src/monitor_routine.c \
		src/philosopher_routine.c \
		src/thread_utils.c \
		src/ft_atoi.c \

OBJS     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) main.c
	$(CC) $(CFLAGS) $(OBJS) main.c -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean:
	rm -rf $(OBJS)
	rm -rf $(NAME)

re: fclean all clean

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --show-leak-kinds=definite ./philo

.PHONY: all clean fclean allClean re 