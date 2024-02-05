#include "CmdJoin.hpp"

CmdJoin::CmdJoin(const int &pFd, const std::string pMessage): ACmd(pFd, pMessage)
{
	if (this->params.size() >= 1)
	{
		std::stringstream paramChan(this->params[0]);
		std::string channel;

		while (std::getline(paramChan, channel, ','))
		{
			this->channels.push_back(channel);
		}
	}
	if (this->params.size() >= 2)
	{
		std::stringstream paramKey(this->params[1]);
		std::string key;

		while (std::getline(paramKey, key, ','))
		{
			this->keys.push_back(key);
		}
	}
}

void CmdJoin::execute(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	if (this->params.size() == 0)
	{
		clientList.find(this->fd)->second->addMsg(
				this->createErrorMsg(
					ERR_NEEDMOREPARAMS,
					this->getClientNick(clientList) + " " + std::string("JOIN"),
					ERR_NEEDMOREPARAMS_STR));
		return;
	}
	else
	{
		int counter = 0;

		for (std::vector<std::string>::iterator it = this->channels.begin();
				it != this->channels.end();
				++it)
		{
			this->params.clear();
			this->params.push_back(*it);
			try
			{
				this->params.push_back(this->keys.at(counter));
			}
			catch(std::exception &e)
			{
				this->params.push_back(std::string());
			}
			std::string errorMsg = this->checkError(clientList, channelList);
			if (errorMsg.length())
				clientList.find(this->fd)->second->addMsg(errorMsg);
			else
			{
				Client *currentClient = clientList.find(this->fd)->second;
				if (!checkChannelUnicity(channelList))
				{
					Channel *newChannel = new Channel(this->params);
					newChannel->addOperator(currentClient);
					newChannel->addUser(currentClient);
					channelList.insert(std::make_pair(this->params[0], newChannel));
				}
				else
				{
					Channel * chanToJoin = channelList.find(this->params[0])->second;
					if (chanToJoin->isInvited(currentClient))
						chanToJoin->removeInvited(currentClient);
					chanToJoin->addUser(currentClient);
				}
				counter++;
			}
		}
	}
}

std::string CmdJoin::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	std::map<std::string, Channel *>::iterator it = channelList.find(this->params[0]);

	if (!this->isClientRegistered(clientList))
		return (this->createErrorMsg(
					ERR_NOTREGISTERED,
					this->getClientNick(clientList),
					ERR_NOTREGISTERED_STR));
	if (params[0][0] != '#')
		return (this->createErrorMsg(
					ERR_NOSUCHCHANNEL,
					this->getClientNick(clientList) + " " + this->params[0],
					ERR_NOSUCHCHANNEL_STR));
	if (!isPrint(this->params[0]))
		return (this->createErrorMsg(
					ERR_INVALIDCHAR,
					this->getClientNick(clientList) + " " + this->params[0],
					ERR_INVALIDCHAR_STR));
	if (it != channelList.end())
	{
		Channel *currentChannel = it->second;
		if (currentChannel->getChannelMaxSize() > 0 && currentChannel->getUsersSize() >= currentChannel->getChannelMaxSize())
			return (this->createErrorMsg(
						ERR_CHANNELISFULL,
						this->getClientNick(clientList) + " " + this->params[0],
						ERR_CHANNELISFULL_STR));
		std::string channelPass = currentChannel->getChannelPassword();
		if (channelPass.length() && (!this->params[1].length() || channelPass != this->params[1])) return (this->createErrorMsg(
						ERR_PASSWDMISMATCH,
						this->getClientNick(clientList),
						ERR_PASSWDMISMATCH_STR));
		if (currentChannel->isInviteOnly() && !currentChannel->isInvited(clientList.find(this->fd)->second))
			return (this->createErrorMsg(
						ERR_INVITEONLYCHAN,
						this->getClientNick(clientList) + " " + currentChannel->getChannelName(),
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
