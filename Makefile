# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/31 17:29:24 by mlitvino          #+#    #+#              #
#    Updated: 2025/02/18 22:23:00 by mlitvino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS =  -I$(LIBFT_DIR)/$(INCLD_DIR) -Wall -Wextra -Werror
# -Wall -Wextra -Werror

NAME = fdf
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx_Linux.a

LIBFT_DIR = ./libft
MLX_DIR = ./minilibx-linux
INCLD_DIR = ./includes

SRC = checks.c draw.c fdf.c get_map.c hooks.c inits.c utils.c
INCLD = fdf.h

OBJ = $(SRC:%.c=%.o)

.SECONDARY: $(OBJ)

all: $(LIBFT) $(NAME)

%.o: %.c $(INCLD)
	$(CC) $(CFLAGS) -I/usr/include -Iminilibx-linux -O3 -c $< -o $@

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -Lminilibx-linux -lmlx_Linux -L/usr/lib -Iminilibx-linux -lXext -lX11 -lm -lz -o $@

$(LIBFT):
	make -C $(LIBFT_DIR) all
	make -C $(LIBFT_DIR) bonus

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

run: $(NAME) clean
	./$(NAME) test.fdf
# DEL

.PHONY: all clean fclean re
