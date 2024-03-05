CC = cc
NAME = philo
SRCS = philosophers.c \
			utils.c \
			routine.c \
			init.c \
			actions.c \
			ft_calloc.c
HEADER = philosophers.h
OBJS = $(SRCS:.c=.o)
CFLAGS = -Werror -Wextra -Wall -g

all:$(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@printf "\033[1;32mCompilation Finished!\n\033[0m"

%.o : %.c $(HEADER)
	@$(CC) $(CFLAGS) $< -c -o $@

fclean: clean
	@rm -f $(NAME)
	@printf "\033[1;35mFiles Deleted!\n\033[0m"

clean:
	@rm -f $(OBJS)
	@printf "\033[1;35mObjects Deleted!\n\033[0m"

re: fclean all

.PHONY: all fclean clean re
