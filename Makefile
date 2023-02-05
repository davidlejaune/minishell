SRCS_FILES = main.c
SRCS_DIR = srcs/
SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g3 -fsanitize=address

LIBFT = libft/libft.a
NAME = minishell

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -Iincludes -o $(NAME) $(OBJS) $(LIBFT)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(LIBFT):
	make -C libft

bonus: all

.PHONY: all clean fclean re bonus
