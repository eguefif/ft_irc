#include "CmdMode.hpp"

CmdMode::CmdMode(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage)
{
	std::vector<std::string>::iterator it = this->params.begin();
	if (this->params.size() >= 1)
	{
		this->channel = std::string(*it);
		++it;
	}
	if (this->params.size() >= 2)
	{
		this->addFlags(*it);
		++it;
	}
	if (this->params.size() >= 3)
	{
		for (; it != this->params.end(); ++it)
		{
			this->args.push(*it);
		}
	}
}

void CmdMode::addFlags(std::string str)
{
	bool toSet;

	if (!str.length())
		return;
	for(std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (*it == '+' || *it == '-')
		{
			if (*it == '+')
				toSet = true;
			if (*it == '-')
				toSet = false;
		}
		else
		{
			if (MODE_LETTERS.find(*it) != std::string::npos)
			{
				std::string letter = "";
				letter += *it;
				if (toSet)
					flags.push_back("+" + letter);
				else
					flags.push_back("-" + letter);
			}
		}
	}
}

void CmdMode::execute(std::map<int, Client *> &clientList,
				std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsgNonRegistered(errorMsg);
	else 
	{
		std::map<std::string, Channel *>::iterator itChannel = channelList.find(this->channel);
		if (itChannel == channelList.end())
			return;
		Channel *currentChannel = itChannel->second;
		for (std::vector<std::string>::iterator it = this->flags.begin();
				it != this->flags.end();
				++it)
		{
			this->handleFlag(*it, currentChannel);
		}
		std::string msg = this->getBroadcastMsg();
		currentChannel->broadcast(msg);
	}
}

std::string CmdMode::getBroadcastMsg()
{
	std::string retval;

	std::vector<std::string>::iterator it = this->params.begin();
	retval += "MODE " + *it + " ";
	++it;
	retval += *it;
	++it;
	if (it != this->params.end())
		retval += " :";
	for (; it != this->params.end(); ++it)
	{
		retval += *it;
		if (it != this->params.end() - 1)
			retval += " ";
	}
	return retval;
}
std::string CmdMode::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	if (!this->params.size())
	{
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " + "PASS",
			ERR_NEEDMOREPARAMS_STR);
	}
	else if (this->channel.length() && channelList.find(this->channel) == channelList.end())
	{
		return this->createErrorMsg(
			ERR_NOSUCHCHANNEL,
			this->getClientNick(clientList) + " " + "MODE",
			ERR_NOSUCHCHANNEL_STR);
	}
	else if (!this->flags.size())
	{
		return this->createErrorMsg(
			RPL_CHANNELMODEIS,
			this->getClientNick(clientList) + " " + this->channel,
			this->getChannelMode(channelList));
	}
	Channel *channel = channelList.find(this->channel)->second;
	if (!channel->isUserOp(clientList.find(this->fd)->second))
	{
		return this->createErrorMsg(
			ERR_CHANOPRIVSNEEDED,
			this->getClientNick(clientList) + " " + this->channel,
			ERR_CHANOPRIVSNEEDED_STR);
	}
	return std::string();
}

void CmdMode::handleFlag(std::string str, Channel *currentChannel)
{
	bool toSet;

	if (str[0] == '+')
		toSet = true;
	else
		toSet = false;
	switch (str[1])
	{
		case 'i': currentChannel->setInviteOnly(toSet);
				  break;
		case 't': currentChannel->setTopicOp(toSet);
				  break;
		case 'k': currentChannel->setPassword(toSet, this->getNextArg());
				  break;
		case 'o': currentChannel->setOperators(toSet, this->getNextArg());
				  break;
		case 'l': currentChannel->setLimit(toSet, this->getNextArg());
				  break;
	}
}

std::string CmdMode::getNextArg()
{
	std::string retval;

	if (this->args.size())
	{
		retval = this->args.front();
		this->args.pop();
	}
	return retval;
}

std::string CmdMode::getChannelMode(std::map<std::string, Channel *> channelList)
{
	std::map<std::string, Channel *>::iterator it = channelList.find(this->channel);
	std::string retval;

	if (it != channelList.end())
	{
		Channel *channel = it->second;
		retval = channel->getModeString();
	}
	return retval;
}
