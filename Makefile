# Makefile
NAME    = philo
LIB     = libphilo.a

CC      = cc
CFLAGS  = -pthread -Wall -Wextra -Werror -g
LIBS    = libphilo.a

LIBFTDIR = libft
LIBFT    = $(LIBFTDIR)/libft.a

SRC = 	src/init.c \
		src/monitor_routine.c \
		src/philosopher_routine.c \
		src/thread_utils.c

OBJS     = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(LIB): $(OBJS) 
	cp $(LIBFT) $(LIB)
	ar rcs $(LIB) $(OBJS)

$(NAME): $(LIBFT) $(LIB) main.c
	$(CC) $(CFLAGS) main.c $(LIBS) $(LDLIBS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean:
	rm -rf $(OBJS)
	rm -f $(NAME) $(LIB)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all clean

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --show-leak-kinds=definite ./minishell

.PHONY: all clean fclean allClean re \
	compileTest1 compileTest2 compileTest3 compileTest4 compileTest5 compileTest6 compileTest6New \
	compileTestExit compileTestPipelineParser compileTestBuiltins compileTestFull