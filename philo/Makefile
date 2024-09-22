FILES= main.c utils.c init_mtx.c assistant.c

OBJ= $(FILES:.c=.o)

NAME= philo
CC= cc -Werror -Wall -Wextra -fsanitize=address -g
RM= rm -rf


all :$(NAME)

$(NAME): philo.h $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c philo.h
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJ)
fclean: clean
	$(RM) $(NAME)

re: fclean all