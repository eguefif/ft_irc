#include "CmdJoin.hpp"

CmdJoin::CmdJoin(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage)
{
}

void CmdJoin::execute(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);
	if (errorMsg.length())	
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		Client *currentClient = clientList.find(this->fd)->second;
		if (!checkChannelUnicity(channelList))
		{
			Channel*	newChannel = new Channel(this->params);
			newChannel->addUser(currentClient);
			newChannel->addOperator(currentClient);
			channelList.insert(std::make_pair(this->params[0], newChannel));
		}
		else
			channelList.find(this->params[0])->second->addUser(currentClient);
	}
}

std::string CmdJoin::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	(void) clientList;
	(void) channelList;
	if (!this->params.size())
		return (this->createErrorMsg(
					ERR_NEEDMOREPARAMS,
					"",
					ERR_NEEDMOREPARAMS_STR));
	return std::string();
}

bool CmdJoin::checkChannelUnicity(std::map<std::string, Channel*> &channelList)
{
	std::string channelName = this->params[0];
	if (channelList.find(channelName) == channelList.end())
			return false;
	return true;
}
