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
			if (std::string(MODE_LETTERS).find(*it) != std::string::npos)
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
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else 
	{
		std::string msg;
		Channel *currentChannel = channelList.find(this->channel)->second;
		if (this->params.size())
		{
		 msg = this->createErrorMsg(
			RPL_CHANNELMODEIS,
			this->getClientNick(clientList) + " " + this->channel,
			this->getChannelMode(channelList));
		}
		else
		{
			this->replyMsg = std::string(":") + this->getClientNick(clientList) + " " + "MODE";
			this->replyParams = " :";

			for (std::vector<std::string>::iterator it = this->flags.begin();
					it != this->flags.end();
					++it)
			{
				std::string wrongNickname = this->handleFlag(*it, currentChannel);
				if (wrongNickname.length())
				{
					std::string msg = this->createErrorMsg(
							ERR_WASNOSUCHNICK,
							this->getClientNick(clientList) + " " + wrongNickname,
							ERR_WASNOSUCHNICK_STR);
					clientList.find(this->fd)->second->addMsg(msg);
				}

			}
			if (this->replyParams.length() > 2)
				 msg = this->replyMsg + this->replyParams;
			else
				 msg = this->replyMsg;
		}
		currentChannel->broadcast(msg);
	}
}

std::string CmdMode::getBroadcastMsg(std::string nickname)
{
	std::string retval;

	std::vector<std::string>::iterator it = this->params.begin();
	retval += ":" + nickname + " " + "MODE ";
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
	if (!this->isClientRegistered(clientList))
		return (this->createErrorMsg(
					ERR_NOTREGISTERED,
					this->getClientNick(clientList),
					ERR_NOTREGISTERED_STR));
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
	}
	Channel *channel = channelList.find(this->channel)->second;
	if (!this->checkUserInChan(clientList.find(this->fd)->second, this->channel, channelList))
	{
		return this->createErrorMsg(
			ERR_NOTONCHANNEL,
			this->getClientNick(clientList) + " " + this->channel,
			ERR_NOTONCHANNEL_STR);
	}
	if (!channel->isUserOp(clientList.find(this->fd)->second))
	{
		return this->createErrorMsg(
			ERR_CHANOPRIVSNEEDED,
			this->getClientNick(clientList) + " " + this->channel,
			ERR_CHANOPRIVSNEEDED_STR);
	}
	return std::string();
}

std::string CmdMode::handleFlag(std::string str, Channel *currentChannel)
{
	bool toSet;
	std::string retval;
	std::string nick;
	std::string limit;

	if (str[0] == '+')
		toSet = true;
	else
		toSet = false;
	switch (str[1])
	{
		case 'i': currentChannel->setInviteOnly(toSet);
				  this->replyMsg += " " + str;
				  break;
		case 't': currentChannel->setTopicOp(toSet);
				  this->replyMsg += " " + str;
				  break;
		case 'k': currentChannel->setPassword(toSet, this->getNextArg());
				  this->replyMsg += " " + str;
				  break;
		case 'o': nick = this->getNextArg();
				  if (!currentChannel->setOperators(toSet, nick))
				  	retval = nick;
				  else
				  {
				  this->replyMsg += " " + str;
					this->replyParams += nick + " ";
				  }
				  break;
		case 'l': limit = this->getNextArg();
				  currentChannel->setLimit(toSet, limit);
				  this->replyMsg += str;
				  this->replyParams += limit + " ";
				  break;
	}
	return retval;
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

bool CmdMode::checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	std::cout << "Test: " <<  channel->isUserInChan(user) << std::endl;
	return channel->isUserInChan(user);
}
