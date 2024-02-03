NAME=ircserv

CC=c++
CFLAGS = -Wall -Wextra -Werror -g #-std=c++98 -fsanitize=address 

_INC = Log.hpp Server.hpp Client.hpp Channel.hpp ACmd.hpp CmdNick.hpp CmdUser.hpp \
	   CmdPass.hpp CmdJoin.hpp CmdInvite.hpp CmdQuit.hpp stringUtils.hpp CmdTopic.hpp

_SRC = main.cpp Server.cpp ServerRun.cpp Client.cpp Channel.cpp Log.cpp ACmd.cpp \
	   CmdNick.cpp CmdUser.cpp CmdPass.cpp CmdJoin.cpp CmdInvite.cpp cmdFactory.cpp \
	   stringUtils.cpp CmdQuit.cpp CmdTopic.cpp

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

run: all
	./ircserv 6667 test

.PHONY: fclean clean re all obj_dir run

test: all
	./test/make_test.sh

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

obj_dir:
	@mkdir -p $(ODIR)

