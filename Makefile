# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgannoun <bgannoun@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/26 23:37:05 by bgannoun          #+#    #+#              #
#    Updated: 2023/07/26 23:37:06 by bgannoun         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell
RM		=	rm -rf
CFLAGS	=	-Wall -Wextra -Werror
OBJ		=	$(SRC:.c=.o)

SRC		=	minishell.c parssing/lexer.c parssing/lexer_2.c parssing/lexer_tools.c parssing/check_syntax.c parssing/parss_red.c\
					parssing/parssing.c parssing/parssing_tools.c parssing/tools.c parssing/tools_2.c parssing/expand_var.c parssing/expand_var_tools.c\
					builtin/cd.c builtin/echo.c builtin/env.c builtin/exit.c builtin/export.c builtin/pwd.c builtin/unset.c builtin/export_tools.c\
					exucution/exucution.c exucution/exucution_utils.c exucution/pipes.c exucution/pipes_utils.c exucution/exucution_2.c\
					exucution/redirection.c exucution/red_utils.c minishell_tool.c

all		:	$(NAME)

$(NAME)	:	$(OBJ)
	cd Libft && make
	$(CC) -lreadline $(CFLAGS) $(OBJ) -o $(NAME) ./Libft/libft.a

clean	:
	$(RM) $(OBJ)
	cd Libft && make clean

fclean	:	clean
	$(RM) $(NAME)
	cd Libft && make fclean

re		:	fclean all
