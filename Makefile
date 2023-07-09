NAME = minishell

VAR_LIBFT = Libft/libft.a
SRCS =  parssing.c builtin.c tools.c parssing_tools.c builtin_utils.c tools_2.c main.c minishell.c

CC = cc

CFLAGS = -Wall -Werror -Wextra 

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(VAR_LIBFT)
		$(CC) $(CFLAGS) $(VAR_LIBFT) $(OBJS) -o $(NAME) -lreadline -ltermcap
clean :
	rm -rf $(OBJS)

$(VAR_LIBFT) :
	make -C Libft

fclean : clean
	rm -rf $(NAME)

re : fclean all