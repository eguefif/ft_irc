#include "../includes/CmdTopic.hpp"

CmdTopic::CmdTopic(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdTopic::execute(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);
	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		if (this->params.size() == 1)
		{
			Channel *currentChannel = channelList.find(this->params[0])->second;
			if (currentChannel->getTopic().length() == 0)
			{
				std::string topicEmptyReply = this->createReplyMsg(
					RPL_NOTOPIC,
					this->getClientNick(clientList) + " " + this->params[0],
					RPL_NOTOPIC_STR);
				clientList.find(this->fd)->second->addMsg(topicEmptyReply);
			}
			else
			{
				std::string topicReply = this->createReplyMsg(
					RPL_TOPIC,
					this->getClientNick(clientList) + " " + this->params[0],
					currentChannel->getTopic());
				clientList.find(this->fd)->second->addMsg(topicReply);
			}
		}
		else
		{
			std::string newTopic;
			std::string newTopicReply;
			Channel *currentChannel = channelList.find(this->params[0])->second;
			for (std::vector<std::string>::iterator it = this->params.begin() + 1; it != this->params.end(); ++it)
			{
				if (it + 1 == this->params.end())
					newTopic += *it;
				else
					newTopic += *it + " ";
			}
			currentChannel->setTopic(newTopic);
			newTopicReply += ":" + this->getClientNick(clientList) + " TOPIC " + this->params[0] + " " + ":" + newTopic;
			currentChannel->broadcast(newTopicReply);
		}
	}
}

std::string CmdTopic::checkError(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	if (!this->params.size())
	{
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " + this->command,
			ERR_NEEDMOREPARAMS_STR);
	}
	if (channelList.find(this->params[0]) == channelList.end())
		return (this->createErrorMsg(
			ERR_NOSUCHCHANNEL,
			this->getClientNick(clientList) + " " + this->params[0],
			ERR_NOSUCHCHANNEL_STR));
	if (!this->checkUserInChan(clientList.find(this->fd)->second, this->params[0], channelList))
		return (this->createErrorMsg(
			ERR_NOTONCHANNEL,
			this->params[0],
			ERR_NOTONCHANNEL_STR));
	if (this->params.size() >= 2 && channelList.find(this->params[0])->second->isTopicOp() && !this->checkUserIsOp(
							clientList.find(this->fd)->second, this->params[0], channelList))
		return (this->createErrorMsg(
			ERR_CHANOPRIVSNEEDED,
			this->params[0],
			ERR_CHANOPRIVSNEEDED_STR));
	return std::string();
}

bool CmdTopic::checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserInChan(user);
}

bool CmdTopic::checkUserIsOp(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserOp(user);
}
