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
			newChannel->addOperator(currentClient);
			newChannel->addUser(currentClient);
			channelList.insert(std::make_pair(this->params[0], newChannel));
		}
		else
			channelList.find(this->params[0])->second->addUser(currentClient);
	}
}

std::string CmdJoin::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	Client *currentClient = clientList.find(this->fd)->second;
	std::map<std::string, Channel *>::iterator it = channelList.find(this->params[0]);
	if (!this->isClientRegistered(clientList))
		return (this->createErrorMsg(
					ERR_NOTREGISTERED,
					"",
					ERR_NOTREGISTERED_STR));
	if (!this->params.size())
		return (this->createErrorMsg(
					ERR_NEEDMOREPARAMS,
					"",
					ERR_NEEDMOREPARAMS_STR));
	if (params[0][0] != '#')
		return (this->createErrorMsg(
					ERR_NOSUCHCHANNEL,
					this->params[0],
					ERR_NOSUCHCHANNEL_STR));
	if (it != channelList.end())
	{
		Channel *currentChannel = it->second;
		if (currentChannel->getChannelMaxSize() != 0 && currentChannel->getUsersSize() >= currentChannel->getChannelMaxSize())
			return (this->createErrorMsg(
						ERR_CHANNELISFULL,
						this->params[0],
						ERR_CHANNELISFULL_STR));
		if (currentChannel->isInviteOnly() && !currentChannel->isInvited(currentClient))
			return (this->createErrorMsg(
						ERR_INVITEONLYCHAN,
						this->params[0],
						ERR_INVITEONLYCHAN_STR));
	}
	return std::string();
}

bool CmdJoin::checkChannelUnicity(std::map<std::string, Channel*> &channelList)
{
	std::string channelName = this->params[0];
	if (channelList.find(channelName) == channelList.end())
			return false;
	return true;
}
