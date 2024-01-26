#include "CommandGenerator.hpp"

CommandGenerator::CommandGenerator()
{
	this->commandList[0] = "NICK";
	this->commandList[1] = "USER";
}

void CommandGenerator::setFd(const int &pFd)
{
	this->senderFd = pFd;
}

ACmd *CommandGenerator::update(const std::string &pMsg)
{
	this->msg += pMsg;
	if (this->msg[this->msg.length()] == '\n' || this->msg.length() >= 512)
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
			return new CmdNick(this->senderFd, this->msg);
			break;
	}
	return 0;
}

int CommandGenerator::getIndex(const std::string &cmd)
{
	for (int i = 0; i < CMD_LIST_NUMBER; ++i)
		if (commandList[i] == cmd)
			return i;
	return -1;
}
