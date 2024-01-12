NAME 		= kdo_editor

CC 			= cc
RM			= rm -f
CLONE 		= git clone --depth=1
CFLAGS 		+= -Wall -Wextra -Werror
CLINKS		= -lreadline

FT			= libft
LIBFT		= $(FT)/libft.a

SRC 		= $(wildcard *.c)
OBJ 		= $(SRC:.c=.o)

all: $(NAME)

bonus: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(CLINKS)

$(LIBFT): $(FT)
	$(MAKE) -C $(FT)

$(FT):
	$(CLONE) https://github.com/kodokaii/libft_plus_plus.git $(FT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if [ -d "$(FT)" ]; then $(MAKE) clean -C $(FT); fi
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(LIBFT)
	$(RM) $(NAME)

clear: fclean
	$(RM) -r $(FT)

re: fclean all

.PHONY:		all bonus clear clean fclean re

