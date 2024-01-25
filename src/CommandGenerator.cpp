#include "CommandGenerator.hpp"

CommandGenerator::CommandGenerator(const int &senderFd. std::map<int, Client*> &pClientList): senderFd(pSenderFd), clientList(pClientList){}

ACmd CommandGenerator::update(const std::string &pMsg)
{
	this->msg += pMsg;
	if (this->msg[this->msg.length()] == '\n')
		return (this->generate());
	return 0;
}

ACmd *CommandGenerator::generate()
{
	std::string cmd;
	std::stringstream ssMsg(this->msg);

	ssMsg >> cmd;
	switch (this->getIndex(cmd))
	{
		case 0:
			return new CmdNick(this->senderFd, this->clientList, ssMsg);
			break;
	}
}

int CommandGenerator::getIndex(const std::string &cmd)
{
	for (int i = 0; i < CMD_LIST_NUMBER; ++i)
		if (commandList[i] == cmd)
			return i;
	return -1;
}
