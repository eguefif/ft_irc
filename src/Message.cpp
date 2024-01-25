#include "CommandGenerator.hpp"

CommandGenerator::CommandGenerator(const int &pSenderFd, std::map<int, Client*> &pClientList): senderFd(pSenderFd), clientList(pClientList) {}

CommandGenerator *CommandGenerator::update(const std::string &pMsg)
{
	this->msg += pMsg;
}

void CommandGenerator::run()
{
	std::string cmd;
	if ()
}
