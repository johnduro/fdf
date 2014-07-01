

INC=/usr/include

INCLIB=$(INC)/../lib

CC=gcc

CFLAGS= -I$(INC) -O3 -I /usr/local/include -I libft/includes

NAME= fdf

SRC = main.c

OBJ = $(SRC:.c=.o)

all: build $(NAME)

build:
	@make -C libft

$(NAME)	:$(OBJ)
	$(CC) -o $(NAME) $(OBJ) -L /usr/local/lib -lmlx -lbsd -L$(INCLIB) -lXext -lX11 -lm -L libft -lft

clean:
	make -C libft clean
	rm -f $(OBJ) *~ core *.core

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re	: fclean all
