#include <string>

#include "CmdNick.hpp"

# define CMD_LIST_NUMBER 2

int getIndex(const std::string &cmd);

ACmd *cmdFactory(std::string msg, int senderFd)
{
	std::stringstream ssMsg(msg);
	std::string cmd;

	ssMsg >> cmd;
	switch (getIndex(cmd))
	{
		case 0:
			return new CmdNick(senderFd, msg);
			break;
	}
	return 0;
	
}

int getIndex(const std::string &cmd)
{
	std::string commandList[] = {"NICK", "USER"};

	for (int i = 0; i < CMD_LIST_NUMBER; ++i)
		if (commandList[i] == cmd)
			return i;
	return -1;
}

