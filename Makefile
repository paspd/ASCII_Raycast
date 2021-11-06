# # **************************************************************************** #
# #                                                                              #
# #                                                         :::      ::::::::    #
# #    Makefile                                           :+:      :+:    :+:    #
# #                                                     +:+ +:+         +:+      #
# #    By: ldauga <ldauga@student.42lyon.fr>          +#+  +:+       +#+         #
# #                                                 +#+#+#+#+#+   +#+            #
# #    Created: 2021/02/08 12:36:34 by leodauga          #+#    #+#              #
# #    Updated: 2021/02/19 09:50:30 by ldauga           ###   ########lyon.fr    #
# #                                                                              #
# # **************************************************************************** #

NAME			=	cub3D

MINILIBX		=	libmlx.a

SRCS			=	main.c

OBJS			=	${SRCS:.c=.o}

INCLUDES		=	./incs

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror -O3 -g3

all:	compilation $(NAME)

compilation:
		@make -C mlx

.c.o:
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS)
			@$(CC) -Lmlx -lmlx -framework OpenGL -framework AppKit $(CFLAGS) -I $(INCLUDES) $(LIB) $(OBJS) -o $(NAME)
			@cp mlx/libmlx.dylib .

clean:
		@rm -f $(OBJS)

norme:
		@norminette srcs/${SRCS_NAME}

fclean:	clean
		@rm -f $(NAME)

re:	fclean all

.PHONY:	all fclean clean re norme
