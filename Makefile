NAME=ircserv

CC=c++
CFLAGS = -Wall -Wextra -Werror #-std=c++98 -fsanitize=address

_INC = Log.hpp Server.hpp Client.hpp ACmd.hpp CmdNick.hpp CmdPass.hpp stringUtils.hpp

_SRC = main.cpp Server.cpp ServerRun.cpp Client.cpp Log.cpp ACmd.cpp CmdNick.cpp CmdPass.cpp cmdFactory.cpp stringUtils.cpp

_OBJ = $(_SRC:.cpp=.o)
SDIR = ./src/
ODIR = ./src/obj/
IDIR = ./includes/
OBJ = $(addprefix $(ODIR), $(_OBJ))
SRC = $(addprefix $(SDIR), $(_SRC))
INC = $(addprefix $(IDIR), $(_INC))

all: obj_dir $(NAME) 

$(NAME): $(OBJ) $(INC)
	$(CC) $(OBJ) -o $@ $(CFLAGS)

$(ODIR)%.o: $(SDIR)%.cpp
	$(CC) -c $< -o $@ -I$(IDIR) $(CFLAGS)

.PHONY: fclean clean re all obj_dir

test: re
	./test/make_test.sh

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

obj_dir:
	@mkdir -p $(ODIR)
