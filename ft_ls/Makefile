NAME = ft_ls.a

SRCS = ./*.c 

OBJS = $(SRCS:.c=.o)

HDRS = ./*.h

FLAGS = -Wall -Wextra -Werror

$(NAME):
		gcc -c $(FLAGS) $(SRCS) -o $(NAME)
		ar rc $(NAME) $(OBJS)
		ranlib $(NAME)

all:	$(NAME)

clean:
		rm -f $(OBJS)

fclean:     clean
		rm -f $(NAME)

re:		fclean all

.PHONY:	all clean fclean re