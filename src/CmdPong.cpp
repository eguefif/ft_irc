#include "CmdPong.hpp"

CmdPong::CmdPong(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdPong::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsgNonRegistered(errorMsg);
	else
	{
		Log::out("Ping received " + this->getClientAddr(clientList));
		clientList.find(this->fd)->second->addMsg(
				SERVER_PREFIX + " " + "PONG" + " " + this->getStringParams());
	}
}

std::string CmdPong::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	(void) channelList;
	(void) clientList;
	return std::string();
}
