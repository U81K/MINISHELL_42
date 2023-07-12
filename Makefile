NAME = minishell

VAR_LIBFT = Libft/libft.a
SRCS = minishell.c parssing/lexer.c parssing/lexer_2.c parssing/lexer_tools.c parssing/check_syntax.c \
					parssing/parssing.c parssing/parssing_tools.c parssing/tools.c parssing/tools_2.c \
					exucution/builtin.c builtin_utils.c

CC = cc

CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g


OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) $(VAR_LIBFT) Makefile
		$(CC) $(CFLAGS) $(VAR_LIBFT) $(OBJS) -o $(NAME) -lreadline -ltermcap
clean :
	rm -rf $(OBJS)
	make clean -C Libft

$(VAR_LIBFT) :
	make -C Libft

fclean : clean
	rm -rf $(NAME)
	make fclean -C Libft

re : fclean all
	make re -C Libft