SRC = srcs/*.c
NAME = ft_ls

all: $(NAME)

$(NAME):
		gcc -Wextra -Werror -Wall $(SRC) -o $(NAME)

clean:

fclean:	clean
		rm -rf $(NAME)

re:		fclean all

.PHONY:	all clean fclean re