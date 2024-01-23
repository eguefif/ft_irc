NAME=ircserv

CC=c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

_INC =

_SRC = main.cpp

_OBJ = $(_SRC:.cpp=.o)
SDIR = ./src/
ODIR = ./src/obj/
IDIR = ./includes/
OBJ = $(addprefix $(ODIR), $(_OBJ))
SRC = $(addprefix $(SDIR), $(_SRC))
INC = $(addprefix $(IDIR), $(_INC))

all: obj_dir $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

$(ODIR)%.o: $(SDIR)%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: fclean clean re all obj_dir

clean:
	rm -rf $(OBJ)

flcean: clean
	rm -rf $(NAME)

re: fclean all

obj_dir:
	mkdir -p $(ODIR)
