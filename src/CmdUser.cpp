#include "CmdUser.hpp"

CmdUser::CmdUser(const int &pFd, const std::string &pMessage): ACmd(pFd, pMessage) {}

void CmdUser::execute(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	std::string errorMsg = this->checkError(clientList, channelList);

	if (errorMsg.length())
		clientList.find(this->fd)->second->addMsg(errorMsg);
	else
	{
		clientList.find(this->fd)->second->setUser(
				this->params[0],
				this->params[3]);
		if (clientList.find(this->fd)->second->isRegistered())
		{
			std::string welcomeMsg = this->createReplyMsg(RPL_WELCOME,
							this->getClientNick(clientList),
						   	RPL_WELCOME_STR);
			clientList.find(this->fd)->second->addMsg(welcomeMsg);
			Log::out("new client registered: "
					+ this->getClientNick(clientList)
					+ " "
					+ this->getClientAddr(clientList));
		}
	}
}

std::string CmdUser::checkError(std::map<int, Client *> &clientList,
			std::map<std::string, Channel *> &channelList)
{
	(void) channelList;
	if (!clientList.find(this->fd)->second->isAuthenticated())
	{
		return this->createErrorMsg(
			ERR_NOTREGISTERED,
			this->getClientNick(clientList),
			ERR_NOTREGISTERED_STR);
	}
	else if (clientList.find(this->fd)->second->isRegistered())
	{
		return this->createErrorMsg(
			ERR_ALREADYREGISTRED,
			this->getClientNick(clientList) + " " + this->getStringParams(),
			ERR_ALREADYREGISTRED_STR);
	}
	else if (this->params.size() != 4)
	{
		return this->createErrorMsg(
			ERR_NEEDMOREPARAMS,
			this->getClientNick(clientList) + " " + "USER",
			ERR_NEEDMOREPARAMS_STR);
	}
	else if (!isPrint(this->params[0]) || !isPrint(this->params[3]))
	{
		return this->createErrorMsg(
			ERR_INVALIDCHAR,
			this->getClientNick(clientList) + " " + this->getStringParams(),
			ERR_INVALIDCHAR_STR);
	}
	return std::string();
}
