#include "../includes/CmdKick.hpp"

CmdKick::CmdKick(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdKick::execute(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		Channel *channel = channelList.find(this->params[0])->second;
		channel->broadcast(this->createKickMsg(clientList));
		channel->removeClient(this->params[1]);
		Log::out(this->params[1] + " was kicked from " + this->params[0]);
	}
}

std::string CmdKick::checkError(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	if (!this->isClientRegistered(clientList))
		return (this->createErrorMsg(
					ERR_NOTREGISTERED,
					this->getClientNick(clientList),
					ERR_NOTREGISTERED_STR));
	if (this->params.size() < 2)
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " + this->command,
			ERR_NEEDMOREPARAMS_STR);
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
	if (this->params.size() >= 2 && !this->checkUserIsOp(clientList.find(this->fd)->second, this->params[0], channelList))
		return (this->createErrorMsg(
			ERR_CHANOPRIVSNEEDED,
			this->params[0],
			ERR_CHANOPRIVSNEEDED_STR));
	if (this->params.size() >= 2 && !this->checkUserInChan(this->params[1], this->params[0], channelList))
		return (this->createErrorMsg(
			ERR_USERNOTINCHANNEL,
			this->getClientNick(clientList) + " " + this->params[1] + " " + this->params[0],
			ERR_USERNOTINCHANNEL_STR));
	if (this->params.size() >= 2 && !this->checkUserInChan(this->getClientNick(clientList), this->params[0], channelList))
		return (this->createErrorMsg(
			ERR_NOTONCHANNEL,
			this->getClientNick(clientList) + " "+ this->params[1] + " " + this->params[0],
			ERR_NOTONCHANNEL_STR));
	return std::string();
}

std::string CmdKick::createKickMsg(std::map<int, Client *> &clientList)
{
	std::string retval;
	std::string reason;

	retval = ":" + this->getClientNick(clientList) + " ";
	retval += std::string("KICK") + " " + this->params[0] + " " + this->params[1];
	reason = this->getReason();
	if (reason.length())
		retval += " :" + reason;
	else
		retval += " :Was kicked by " + this->getClientNick(clientList) + " with no reason";
	return retval;
}

std::string CmdKick::getReason()
{
	std::string retval;

	if (this->params.size() > 2)
	{
		for (std::vector<std::string>::iterator it = this->params.begin(); it != this->params.end(); ++it)
		{
			retval += *it;
			if (it + 1 != this->params.end())
				retval += " ";
		}
	}
	return retval;
}


bool CmdKick::checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserInChan(user);
}

bool CmdKick::checkUserInChan(std::string user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserInChan(user);
}

bool CmdKick::checkUserIsOp(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserOp(user);
}
