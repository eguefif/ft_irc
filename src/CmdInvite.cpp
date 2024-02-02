#include "../includes/CmdInvite.hpp"

CmdInvite::CmdInvite(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage)
{
}

CmdInvite::~CmdInvite()
{
}

void CmdInvite::execute(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);
	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		channelList.find(this->params[1])->second->addUserInvited(clientList.find(this->fd)->second);
		Log::out(this->getClientNick(clientList) + " " + "invited" + " " + this->params[0] + " " + "to channel" + " " + this->params[1]);
	}
}

std::string CmdInvite::checkError(std::map<int, Client *> &clientList, std::map<std::string, Channel *> &channelList)
{
	if (this->params.size() < 2)
		return (this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " +this->command,
			ERR_NEEDMOREPARAMS_STR));
	if (channelList.find(this->params[1]) == channelList.end())
		return (this->createErrorMsg(
			ERR_NOSUCHCHANNEL,
			this->getClientNick(clientList) + " " + this->params[1],
			ERR_NOSUCHCHANNEL_STR));
	if (!this->checkUserInChan(clientList.find(this->fd)->second, this->params[1], channelList))
		return (this->createErrorMsg(
			ERR_NOTONCHANNEL,
			this->params[1],
			ERR_NOTONCHANNEL_STR));
	if (channelList.find(this->params[1])->second->isInviteOnly() && !this->checkUserIsOp(
							clientList.find(this->fd)->second, this->params[1], channelList))
		return (this->createErrorMsg(
			ERR_CHANOPRIVSNEEDED,
			this->params[1],
			ERR_CHANOPRIVSNEEDED_STR));
	if (this->isAlreadyInChan(this->params[0], this->params[1], channelList))
		return (this->createErrorMsg(
			ERR_USERONCHANNEL,
			this->getClientNick(clientList) + " " + this->params[0] + " " + this->params[1],
			ERR_USERONCHANNEL_STR));
	if (!this->checkIfNickExist(this->params[0], clientList))
		return (this->createErrorMsg(
			ERR_NOSUCHNICK,
			this->getClientNick(clientList),
			ERR_NOSUCHNICK_STR));
	return std::string();
}

bool CmdInvite::checkIfNickExist(std::string nickname, std::map<int, Client *> &clientList)
{
	for (std::map<int, Client *>::iterator it = clientList.begin(); it != clientList.end(); ++it)
	{
		if ((*it).second->getNickname() == nickname)
			return true;
	}
	return false;
}

bool CmdInvite::checkUserInChan(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserInChan(user);
}

bool CmdInvite::checkUserIsOp(Client *user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isUserOp(user);
}

bool CmdInvite::isAlreadyInChan(std::string user, std::string channelName, std::map<std::string, Channel *> &channelList)
{
	Channel *channel = channelList.find(channelName)->second;
	return channel->isAlreadyInChan(user);
}
