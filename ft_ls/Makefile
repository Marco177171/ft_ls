NAME = ft_ls

SRCS = ./srcs/*.c 

OBJS = $(SRCS:.c=.o)

HDRS = ./srcs/*.h

FLAGS = -Wall -Wextra -Werror

$(NAME):
		gcc $(FLAGS) $(SRCS) $(HDRS) -o $(NAME)
		ar rc $(NAME) $(OBJS)
		ranlib $(NAME)

all:	$(NAME)

clean:
		rm -f $(OBJS)

fclean:     clean
		rm -f $(NAME)

re:		fclean all

.PHONY:	all clean fclean re