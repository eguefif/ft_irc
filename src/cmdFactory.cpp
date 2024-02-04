#include <string>

#include "CmdPass.hpp"
#include "CmdNick.hpp"
#include "CmdUser.hpp"
#include "CmdJoin.hpp"
#include "CmdInvite.hpp"
#include "CmdQuit.hpp"
#include "CmdTopic.hpp"
#include "CmdMode.hpp"
#include "CmdKick.hpp"

# define CMD_LIST_NUMBER 10

int getIndex(const std::string &cmd);

ACmd *cmdFactory(std::string msg, int senderFd, std::string password)
{
	std::stringstream ssMsg(msg);
	std::string cmd;

	if (msg[0] == ':')
		ssMsg >> cmd;
	ssMsg >> cmd;
	switch (getIndex(cmd))
	{
		case 0:
			return new CmdNick(senderFd, msg);
			break;
		case 1:
			return new CmdUser(senderFd, msg);
			break;
		case 2:
			return new CmdPass(senderFd, msg, password);
			break;
		case 3:
			return new CmdJoin(senderFd, msg);
			break;
		case 4:
			return new CmdQuit(senderFd, msg);
			break;
		case 5:
			return new CmdInvite(senderFd, msg);
		case 6:
			return new CmdMode(senderFd, msg);
			break;
		case 7:
			return new CmdTopic(senderFd, msg);
			break;
		case 8:
			return new CmdKick(senderFd, msg);
			break;
	}
	return 0;
	
}

int getIndex(const std::string &cmd)
{
	std::string commandList[] = {"NICK", "USER", "PASS", "JOIN", "QUIT", "INVITE",
	"MODE", "TOPIC", "KICK"};

	for (int i = 0; i < CMD_LIST_NUMBER; ++i)
		if (commandList[i] == cmd)
			return i;
	return -1;
}
