#include "CmdQuit.hpp"

CmdQuit::CmdQuit(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdQuit::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	std::map<int, Client *>::iterator clientIt = clientList.find(this->fd);

	if (clientIt != clientList.end())
	{
		std::string msg = this->getQuitMsgBroadcast(this->getClientNick(clientList));
		Client *client = clientIt->second;

		client->disconnect();
		for (std::map<std::string, Channel *>::iterator it = channelList.begin();
				it != channelList.end();
				++it)
		{
			it->second->removeClient(client);
		}
		for (std::map<int, Client *>::iterator it = clientList.begin();
				it != clientList.end();
				++it)
		{
			if (!it->second->isDisconnected())
				it->second->addMsg(msg);
		}
		msg = this->getQuitMsg(this->getClientNick(clientList));
		client->addMsgNonRegistered(msg);
	}
}

std::string CmdQuit::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	(void) clientList;
	(void) channelList;
	return std::string();
}

std::string CmdQuit::getQuitMsgBroadcast(const std::string &nickname)
{
	std::string retval;

	retval += ":" + nickname + " QUIT :quit:";
	if (this->params.size() >= 1)
		retval += this->getStringParams();
	else
		retval += "for unknown reason, maybe he or she wanted to pee";
	return retval;
}

std::string CmdQuit::getQuitMsg(const std::string &nickname)
{
	std::string retval;

	retval += "ERROR: Closing link :";
	retval += "(" + nickname + ") :";
	if (this->params.size() >= 1)
		retval += this->getStringParams();
	return retval;
}
