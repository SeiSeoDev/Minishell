# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dasanter <dasanter@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/07 16:03:33 by tamigore          #+#    #+#              #
#    Updated: 2022/03/02 17:28:53 by dasanter         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

NAME = minishell

LIBFT = libft

DIR_S = srcs/

HEADER = includes/

SOURCES =	minishell.c		\
			usefull_pars.c	\
			free.c			\
			init.c			\
			pars.c			\
			print.c			\
			expend.c		\
			handler.c		\
			ex_env.c		\
			exec.c			\
			ex_built.c		\
			utils.c			\
			cmd.c			\
			redir.c			\
			fork.c			\
			heredoc.c		\
			ex_hit.c

SRCS = $(addprefix $(DIR_S),$(SOURCES))

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Werror -Wextra -I $(HEADER)

FLAGS = -L $(LIBFT) -lft

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(FLAGS) -o $(NAME) -lreadline -g

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

fsan: $(OBJS) $(HEADER)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(FLAGS) -o $(NAME) -lreadline -g3 -fsanitize=address
