# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tamigore <tamigore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/07 16:03:33 by tamigore          #+#    #+#              #
#    Updated: 2021/12/07 16:04:03 by tamigore         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang

NAME = minishell

LIBFT = libft

DIR_S = srcs/

HEADER = includes/

SOURCES =	main.c

SRCS = $(addprefix $(DIR_S),$(SOURCES))

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Werror -Wextra -I $(HEADER)

FLAGS = -L $(LIBFT) -lft

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(FLAGS) -o $(NAME)

norme:
	norminette ./$(LIBFT)
	@echo
	norminette ./$(HEADER)
	@echo
	norminette ./$(DIR_S)

clean:
	@rm -f $(OBJS)
	@make clean -C $(LIBFT)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT)

re: fclean all